## Saber PSN Unlock Tool v0.0.45 — CFW + HEN test packages

Browse PS3 trophies and use the supported, explicitly confirmed unlock flow. The CFW executable completed Persona 5's remaining48 trophies on one owner's console; independent saved-file reads confirmed49/49 earned, including platinum, with the prior earned trophy preserved.

### Downloads

| File | Console | XMB name |
| --- | --- | --- |
| Saber-PSN-Unlock-Tool-v0.0.45-CFW.pkg | CFW | Saber PSN Unlock Tool |
| Saber-PSN-Unlock-Tool-v0.0.45-HEN-TEST.pkg | HEN experimental test | Saber PSN Unlock Tool HEN |

The ZIP contains both packages and instructions. Check PKG hashes against SHA256SUMS.txt. Install only the edition appropriate for your console; enable HEN before launching its edition.

### Checks completed

- Actual finalized packages were extracted and checked for identity, approved artwork, required notices and expected contents.
- Only the XMB title metadata changed; executable bytes remain identical to the previously checked CFW/HEN builds.
- 862 regression tests passed; one environment-specific prerequisite test was skipped. Targeted undefined-behavior checks passed. Both renamed packages reached the ready UI in disconnected RPCS3 without trophy calls or changes to authored trophy files.
- The CFW executable has the one-console local Persona 5 completion result above. The HEN edition has not yet been tested on real HEN hardware.

### Read before testing

This is a development release. **Game-specific setup is required and is not included.** No game packs, session credentials or account data are distributed. Without valid setup, the app can browse/back up supported data but will report that unlocking is not set up; installing this PKG does not enable every game automatically.

**Full-batch PSN sync remains untested.** The application name is not a guarantee of PSN acceptance. Other games, firmware variants and HEN unlocking remain unverified. First HEN tests should cover startup, browsing, controller navigation and Start exit.

**Copy reports and backups outside the app folder before deleting, reinstalling or replacing it.** Keep the console on through each operation, wait for the final result, and never automatically retry an uncertain award. Read README.txt for controls and testing steps.

Public status: [local bulk verification](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/blob/main/docs/PERSONA5-LOCAL-BULK-2026-10-26.md).
