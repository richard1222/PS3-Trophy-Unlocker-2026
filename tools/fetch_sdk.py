"""Acquire the pinned official SDK, verify its hash, and refuse overwrites."""
import argparse
import hashlib
import json
import os
import tempfile
import time
import urllib.request
from pathlib import Path


def fetch(url, output, expected_sha, max_bytes):
    if output.exists():
        raise FileExistsError(str(output))
    if max_bytes <= 0 or len(expected_sha) != 64:
        raise ValueError('Invalid resource bound or SHA-256')
    bytes.fromhex(expected_sha)
    temporary = None
    try:
        digest = hashlib.sha256(); total = 0; start = time.monotonic()
        with tempfile.NamedTemporaryFile(dir=output.parent, prefix='.sdk-', suffix='.partial', delete=False) as sink:
            temporary = Path(sink.name)
            with urllib.request.urlopen(url, timeout=15) as source:
                while True:
                    if time.monotonic() - start > 180:
                        raise TimeoutError('SDK transfer deadline exceeded')
                    chunk = source.read1(65536) if hasattr(source, 'read1') else source.read(65536)
                    if not chunk:break
                    total += len(chunk)
                    if total > max_bytes:raise ValueError('SDK transfer exceeds byte limit')
                    digest.update(chunk);sink.write(chunk)
            sink.flush();os.fsync(sink.fileno())
        if digest.hexdigest() != expected_sha.lower():
            raise ValueError('SDK SHA-256 mismatch')
        # Link creation is atomic and refuses a destination created during download.
        os.link(temporary, output)
        return dict(bytes=total, sha256=digest.hexdigest())
    finally:
        if temporary is not None:temporary.unlink(missing_ok=True)


def main():
    lock=json.loads((Path(__file__).resolve().parents[1]/'toolchain.lock.json').read_text())
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',required=True,type=Path)
    parser.add_argument('--url',default=lock['url'],help='Override for controlled fixtures')
    parser.add_argument('--sha256',default=lock['sha256'])
    parser.add_argument('--max-bytes',default=lock['max_bytes'],type=int)
    args=parser.parse_args()
    try:print(json.dumps(fetch(args.url,args.output,args.sha256,args.max_bytes),sort_keys=True))
    except FileExistsError:
        print('Output exists; no overwrite performed.');return 4
    except Exception as error:
        print(type(error).__name__+': '+str(error));return 3
    return 0


if __name__=='__main__':raise SystemExit(main())
