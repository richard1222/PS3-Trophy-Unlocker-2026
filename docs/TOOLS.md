# Research tools and native probe

## Read-only trophy inspector

Python 3.10+, standard library. From the repository root:

```text
python tools/inspect_trophy.py decoded-TROPUSR.DAT
python tools/inspect_trophy.py decoded-TROPUSR.DAT --output new-report.json
```

Input is read-only, at most 4 MiB; at most 32 tables and 4,096 entries per table. The supported decoded layout is big-endian. The tool checks table bounds/overlap, known entry sizes, duplicate IDs, matching grade/state IDs and value ranges. It maps by explicit ID, including sparse or reordered IDs. It prints stable UTF-8 JSON, or creates a new JSON path and refuses overwrites. Exit 0 means inspected; 2 is CLI usage; 3 is a file/format error; 4 is an existing output. Generic table metadata is not proof that an unknown table's meaning is understood.

No network calls, decryption, protection verification, account/console ID extraction or trophy writes. The same bytes produce the same JSON; the input path and current time are not included.

## Pinned SDK downloader

```text
python tools/fetch_sdk.py --output sdk.tar.gz
```

Requires Python 3.10+, network access and enough free disk space. toolchain.lock.json identifies the official Linux release, SHA-256 and a 350,000,000-byte transfer cap. The transfer has a 15-second I/O timeout and checks a 180-second elapsed deadline between reads. The GitHub job has a 10-minute hard limit. A complete hash-verified temporary file is linked atomically to the new destination; an existing destination is refused. Handled failures remove the temporary file. Exit 0 is success, 2 usage, 3 transfer/hash/bound failure, 4 existing output. Overrides --url, --sha256 and --max-bytes exist for controlled fixtures. Unit tests use synthetic local file URLs, not a substitute SDK.

The downloaded archive is a build dependency, not shipped in project releases. Its Linux executables are run by CI; this does not install WSL or change the Windows host.

## PS3 read-only discovery probe

The source is source/probe.c; make builds trophy-probe.pkg using PSL1GHT. The pinned CI job runs the host tests, verifies/downloads the SDK, compiles and packages the probe, and attaches the PKG plus checksum as a build artifact. A workflow file alone does not establish a successful build.

App ID TROPH2026. On launch it scans numeric local-user directories and NPWR trophy-set directories, reads 48-byte TROPUSR/TROPTRNS headers and creates a new /dev_hdd0/game/TROPH2026/USRDIR/probe-NNN.txt. It exits to XMB and has no graphical menu yet. Reads are bounded by 32 matching users, 1,024 matching sets and directory-entry loop limits. Reports use sequential new-file names and are never overwritten. File-read errors are reported; unreadable or encrypted headers are not declared corrupt based on a missing decoded magic.

The probe does not unlock, decrypt, touch trophy files, change firmware, use syscall8, transmit data or access account/console identifiers. It writes only its own report. Do not install it until a verified build and console-test instructions are supplied. Keep actual console reports private unless their contents have been reviewed for publication.
