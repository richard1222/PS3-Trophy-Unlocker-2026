import importlib.util
import json
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location('inspector', ROOT/'tools/inspect_trophy.py')
m = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(m)


def fixture(ids=(0, 7, 65)):
    # Synthetic decoded TROPUSR layout; not a private console file.
    start = 48 + 2 * 32
    b = bytearray(start + len(ids) * (96 + 112))
    b[:8] = bytes.fromhex('818f54ad00010000')
    struct.pack_into('>I', b, 8, 2)
    struct.pack_into('>IIIIQQ', b, 48, 4, 80, 1, len(ids), start, 0)
    struct.pack_into('>IIIIQQ', b, 80, 6, 96, 1, len(ids), start+len(ids)*96, 0)
    for index, ident in enumerate(ids):
        at = start + index * 96
        struct.pack_into('>IIIIII', b, at, 4, 80, index, 0, ident, (index % 4)+1)
        at = start + len(ids)*96 + index*112
        struct.pack_into('>IIIIIIIIQQ', b, at, 6, 96, index, 0, ident, index % 2, 0x1000 if index % 2 else 0, 0, 123 if index % 2 else 0, 123 if index % 2 else 0)
    return b


class InspectionTests(unittest.TestCase):
    def test_sparse_ids_and_big_endian(self):
        r = m.inspect(fixture())
        self.assertEqual([x['id'] for x in r['trophy_rows']], [0, 7, 65])
        self.assertEqual(r['trophy_rows'][1]['timestamp1'], 123)
        self.assertTrue(r['trophy_rows'][1]['unlocked'])

    def test_reordered_state_rows_map_by_id(self):
        b = fixture();at = 112+3*96
        a, c = bytes(b[at:at+112]), bytes(b[at+224:at+336])
        b[at:at+112], b[at+224:at+336] = c, a
        self.assertEqual(m.inspect(b)['trophy_rows'][1]['id'], 7)

    def test_all_truncated_prefixes_rejected(self):
        b = fixture()
        for n in range(len(b)):
            with self.subTest(n=n), self.assertRaises(m.FormatError):m.inspect(b[:n])

    def test_rejections(self):
        mutations=[(0, '>I', 0), (8, '>I', 33), (64, '>Q', 2**63),
                   (52, '>I', 79), (60, '>I', 4097), (80, '>I', 4),
                   (96, '>Q', 120), (112, '>I', 5), (132, '>I', 99),
                   (112+288+20, '>I', 2), (112+288+16, '>I', 999),
                   (112+96+16, '>I', 0)]
        for offset, fmt, value in mutations:
            b=fixture();struct.pack_into(fmt,b,offset,value)
            with self.subTest(offset=offset), self.assertRaises(m.FormatError):m.inspect(b)

    def test_byte_limit(self):
        with self.assertRaises(m.FormatError):m.inspect(b'\0'*(m.MAX_BYTES+1))

    def test_cli_no_overwrite_determinism_and_input_preservation(self):
        with tempfile.TemporaryDirectory() as temp:
            root=Path(temp);source=root/'TROPUSR.DAT';source.write_bytes(fixture())
            def call(*args):
                return subprocess.run([sys.executable,str(ROOT/'tools/inspect_trophy.py'),str(source),*args],capture_output=True)
            a,b=call(),call()
            self.assertEqual((a.returncode,a.stdout),(b.returncode,b.stdout))
            self.assertEqual(a.returncode,0);json.loads(a.stdout)
            output=root/'report.json'
            self.assertEqual(call('--output',str(output)).returncode,0)
            self.assertEqual(output.read_bytes(),a.stdout)
            self.assertEqual(call('--output',str(output)).returncode,4)
            self.assertEqual(source.read_bytes(),fixture())


if __name__ == '__main__':unittest.main()
