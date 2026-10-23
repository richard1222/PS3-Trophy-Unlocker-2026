# PS3 Trophy Unlocker 2026 — Development Handoff (2026-10-23)

Project owner/lead tester: Richard1222/Saber
Public GitHub: https://github.com/richard1222/PS3-Trophy-Unlocker-2026
Target console: PS3 Evilnat 4.93 D-PEX, webMAN MOD
Current source milestone: v0.0.17 seek diagnostics
Current baseline commit before this handoff: 4af32b8a13881f595d580fc98ae7e0ea67dac8a6

## Purpose and truthful status
This is a READ-ONLY trophy browser/probe effort toward eventual individually selected trophy operations. The verified progress is detection, bounded reads, reports and diagnostics. There is NO verified native context registration, no confirmed selected-trophy unlock, and no PSN synchronization proof. Do not advertise or ship as a working unlocker.

## Verified PS3 evidence
- Trophy Browser starts and lists installed sets on the owner's PS3.
- Persona 5: BLUS31604, trophy set NPWR11866_00, user 00000001, trophy ID 11 = "Merciless Executioner" (perform 50 Persona fusions) verified in installed TROPCONF.SFM metadata.
- v0.0.13 console report: users=1, sets=23, report_complete=1; Persona 5 TROPUSR.DAT recognized with table_count=10; metadata name and ID markers found.
- v0.0.14 console report: complete read of Persona 5 TROPUSR.DAT, 11,712 bytes, FNV-1a 64-bit fingerprint 90adad44cc40cc55. This is a noncryptographic fingerprint, not signature validation.
- v0.0.15: state decoder reports UNKNOWN, unsupported/invalid, with error code zero; file fingerprint unchanged.
- v0.0.16: precise failure "reason=entry_seek table=4 entry=0 error=0x00000000", fingerprint still 90adad44cc40cc55, report_complete=1. Source did not preserve the seek return code, so zero is not proof of a successful seek.
- Native context registration: UNTESTED. Trophy ID 11 actual locked/unlocked state: UNKNOWN.
- Most recent verified console log is v0.0.16, not v0.0.17.

## Latest source changes (v0.0.17)
The source/probe.c test now saves the return value of sysLv2FsLSeek64, records the requested and actual offset, and rejects a mismatch. Diagnostic format:
seek_diagnostic requested=<offset> actual=<offset> return=0x<code> table=<type> entry=<index>
The native state parser remains experimental and its output must be compared with independent evidence. No trophy writes or PSN sync functions added.
GitHub plan: https://github.com/richard1222/PS3-Trophy-Unlocker-2026/blob/main/docs/V0.0.17-SEEK-DIAGNOSTICS.md
Previous diagnostic log: https://drive.google.com/file/d/1UQ7Yuvtb-JCEoWR3FsB7b52kUkU2jE0A/view

## Relevant files
- source/probe.c — installed-set discovery, metadata preflight, bounded data fingerprint, experimental type-4/type-6 state reader and seek diagnostics.
- source/startup.c — PS3 graphical browser and startup workflow.
- tools/inspect_trophy.py — host-side decoded-format parser reference, read-only.
- tools/check_readonly_symbols.py — prohibition check for write-capable trophy symbol references.
- .github/workflows/build.yml — pinned SDK test/build and trophy-startup.pkg artifact.
- docs/V0.0.12-READONLY-CONTEXT-GATE.md — native read-only context lifecycle go/no-go.
- docs/V0.0.13-STATE-READER-STATUS.md, docs/V0.0.14-INTEGRITY-CHECKPOINT.md, docs/V0.0.15-STATE-PARSER-EXPERIMENT.md, docs/V0.0.16-DECODER-DIAGNOSTICS.md, docs/V0.0.17-SEEK-DIAGNOSTICS.md.
- Muse reference PS3-Trophy-Unlocker-Muse-Research.md on Drive: https://drive.google.com/file/d/1Dx2y14YB2iaEYq-FxmR0-U7zWLJMvTUo/view

## Immediate next actions
1. Trigger GitHub Actions workflow on main and confirm it builds the v0.0.17 commit or later. Do not assume a green run applies to the newest code without checking SHA.
2. Download the read-only-probe artifact, extract trophy-startup.pkg, install/launch on the PS3 and run the scan.
3. Retrieve newest trophy-probe-diag-###.txt from /dev_hdd0/game/TROPH2026/USRDIR/ (fallback /dev_hdd0/tmp/troph2026/) and upload to Google Drive.
4. Verify probe_version=0.0.17-seek; inspect seek_diagnostic, trophy_11_state, state_integrity, report_complete. Diagnose any new errors from actual logs.
5. Cross-check the file parser's offsets/entry layout and seek semantics with credible SDK/source documentation and independent console viewer state. Keep results UNKNOWN until validation.
6. Build a separate native read-only context test only after ABI, communication signature, registration requirements and error paths are grounded. Retain a separate go/no-go before any individual unlock experiment.

## Boundaries and safety
- No changes to TROPUSR.DAT, TROPCONF.SFM, PARAM.PFD or trophy folders from the probe. Reports are stored only in app-owned locations or USB.
- Do not attempt arbitrary cross-process context reuse. A process-local context cannot simply be borrowed from a running game.
- No live memory writes, PSN synchronization, or simulated successes.
- Test on one console does not prove general firmware compatibility.
- Preserve user backups and compare original and test data before any later write-capable design.

## Handoff acceptance criteria
A successor can find the code and latest logs, reproduce a green CI build at a specific SHA, run the latest read-only PKG on console, and explain the actual seek/parser failure from evidence. Do not claim a working unlocker without a controlled, documented console result.
