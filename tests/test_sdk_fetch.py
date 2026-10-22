import hashlib
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]


class FetchTests(unittest.TestCase):
    def test_operator_contract(self):
        with tempfile.TemporaryDirectory() as temp:
            base=Path(temp);data=bytes(range(256))*4
            source=base/'source';source.write_bytes(data)
            digest=hashlib.sha256(data).hexdigest()
            def run(out,sha=digest,limit=1024):
                return subprocess.run([sys.executable,str(ROOT/'tools/fetch_sdk.py'),'--url',source.as_uri(),'--sha256',sha,'--max-bytes',str(limit),'--output',str(out)],capture_output=True)
            a,b=base/'a',base/'b'
            self.assertEqual(run(a).returncode,0)
            self.assertEqual(run(b).returncode,0)
            self.assertEqual(a.read_bytes(),b.read_bytes())
            self.assertEqual(run(a).returncode,4)
            self.assertEqual(run(base/'bad','0'*64).returncode,3)
            self.assertFalse((base/'bad').exists())
            self.assertEqual(run(base/'large',limit=1023).returncode,3)
            self.assertFalse((base/'large').exists())
            self.assertEqual(source.read_bytes(),data)
            self.assertFalse(list(base.glob('*.partial')))


if __name__=='__main__':unittest.main()
