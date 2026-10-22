"""Read supported decoded PS3 trophy tables. Never edits or decrypts input."""
import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

MAX_BYTES = 4 * 1024 * 1024
MAX_TABLES = 32
MAX_ENTRIES = 4096


class FormatError(ValueError):
    pass


def inspect(data):
    if not 48 <= len(data) <= MAX_BYTES:
        raise FormatError('File is truncated or exceeds 4 MiB')
    if data[:8] != bytes.fromhex('818f54ad00010000'):
        raise FormatError('Unsupported header: expected decoded trophy data; this tool does not decrypt protected files')
    count = struct.unpack_from('>I', data, 8)[0]
    if not 1 <= count <= MAX_TABLES or 48 + count * 32 > len(data):
        raise FormatError('Invalid table count or truncated table directory')
    tables = []
    ranges = [(0, 48 + 32 * count)]
    seen = set()
    for index in range(count):
        kind, size, unknown, entries, offset, reserved = struct.unpack_from('>IIIIQQ', data, 48 + index * 32)
        if kind in seen or size > MAX_BYTES or entries > MAX_ENTRIES:
            raise FormatError('Duplicate table type or excessive table size/count')
        seen.add(kind)
        length = (size + 16) * entries
        if entries and (offset > len(data) or length > len(data) - offset):
            raise FormatError('Table payload outside file bounds')
        if entries:
            for lo, hi in ranges:
                if offset < hi and lo < offset + length:
                    raise FormatError('Table overlaps another table or directory')
            ranges.append((offset, offset + length))
        tables.append(dict(type=kind, payload_bytes=size, entries=entries, offset=offset))
    by_type = {t['type']: t for t in tables}
    result = dict(size_bytes=len(data), sha256=hashlib.sha256(data).hexdigest(), tables=tables,
                  trophy_rows=None, limits='Read-only decoded-layout inspection; not decryption, PFD verification, firmware compatibility or PSN synchronization validation.')
    if 4 in by_type and 6 in by_type:
        grades, states = {}, {}
        for kind, expected_size, target in [(4, 80, grades), (6, 96, states)]:
            t = by_type[kind]
            if t['payload_bytes'] != expected_size:
                raise FormatError('Unsupported trophy-entry stride')
            for index in range(t['entries']):
                at = t['offset'] + index * (expected_size + 16)
                entry_type, entry_size = struct.unpack_from('>II', data, at)
                ident, value = struct.unpack_from('>II', data, at + 16)
                if entry_type != kind or entry_size != expected_size or ident in target:
                    raise FormatError('Invalid or duplicate trophy entry')
                if kind == 4:
                    if value not in [1, 2, 3, 4]:
                        raise FormatError('Unsupported trophy grade')
                    target[ident] = value
                else:
                    if value not in [0, 1]:
                        raise FormatError('Unsupported trophy state')
                    sync = struct.unpack_from('>I', data, at + 24)[0]
                    ticks = struct.unpack_from('>QQ', data, at + 32)
                    target[ident] = dict(unlocked=bool(value), sync_state=sync,
                                         timestamp1=ticks[0], timestamp2=ticks[1])
        if grades.keys() != states.keys():
            raise FormatError('Grade and state trophy IDs do not match')
        names = {1: 'platinum', 2: 'gold', 3: 'silver', 4: 'bronze'}
        result['trophy_rows'] = [dict(id=i, grade=names[grades[i]], **states[i]) for i in sorted(grades)]
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input', type=Path)
    parser.add_argument('--output', type=Path, help='Optional new JSON file; refuses existing paths')
    args = parser.parse_args()
    try:
        with args.input.open('rb') as stream:
            data = stream.read(MAX_BYTES + 1)
        text = json.dumps(inspect(data), indent=2, sort_keys=True) + '\n'
        if args.output:
            with args.output.open('x', encoding='utf8', newline='\n') as stream:
                stream.write(text)
        else:
            sys.stdout.buffer.write(text.encode('utf8'))
    except FileExistsError:
        print('Output exists; no file was overwritten.', file=sys.stderr)
        return 4
    except (FormatError, OSError) as error:
        print(str(error), file=sys.stderr)
        return 3
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
