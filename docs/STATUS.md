# Latest hardware finding — November 6, 2026

**Known v0.0.62 issue: whole-library preparation returned to XMB before its first worker started.** Keep the saved job and backups; do not repeat preparation or confirm awards pending the fix.

**Proven:** v0.0.62 CFW is installed, with all seven package files independently verified and 47 prior reports preserved. The physical exclusion flow passed: 25 registered sets, exactly four unavailable games reviewed and excluded, 21 included games, 1,024 remaining trophies, 17 matching setups checked, and no included setup gaps. Fifteen uninitialized digital sets were reviewed separately.

The user then chose preparation. Its first worker was queued, but no successor app report appeared. Saved-job decoding confirms no worker claim, no completed preparation, and no award confirmation. Preparation and award result stores are empty.

**Inference:** The failure occurs during the transition to the next process. Its exact cause is still being tested.

**Unknown / next:** A completed whole-library preparation/award run remains unverified. A separate read-only handoff diagnostic passed the disconnected emulator check and is installed for the next hardware test; that test cannot award trophies. HEN hardware remains untested.

The public v0.0.62 release is now marked **Latest**, rather than prerelease. Package assets and SHA-256 values are unchanged.

---

## Earlier release snapshot — superseded where stated above

# Current status — November 6, 2026

**v0.0.62 is the artwork and reviewed-selection test release for Evilnat 4.93 CFW, with a separate HEN package.** [Release and checksums](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases/tag/v0.0.62).

In plain language: selected-game awards work in tested cases. The new optional skip list lets a job leave out games whose matching setup is absent, after explicit review. It does not establish that a whole-library award run is complete or that every game is compatible.

## Proven

- v0.0.61 CFW was installed and independently read back: all seven packaged files matched, all 47 prior reports were unchanged, and app-state/setup directory names were preserved. This was not a fresh byte-for-byte backup of every checkpoint and setup.
- 85 unique application test methods passed without skips. The final native input-guard audit passed address- and undefined-behavior sanitizer checks.
- v0.0.62 changes version markers and the approved XMB artwork while retaining that audited application logic.
- Both v0.0.62 packages passed payload, executable/import, identity, 4.93 metadata, artwork and notice checks. Their actual extracted payloads reached UI startup/discovery in disconnected RPCS3 4.93, with unchanged authored fixtures and no trophy API calls. The host stopped each UI after 20 seconds.
- Super Heroine Chronicle's single-trophy result was independently verified at 14/40 with all 13 previous earned records preserved; the owner confirmed PSN sync.
- World at War's preparation/cancel test reached a fresh confirmation with 0/65 unchanged and no award. Its earlier registration failure remains historical evidence.
- The latest complete hardware library check predates that World at War success: 25 registered sets, 1,268 remaining, five missing matching setups, and 15 separately reviewed uninitialized digital exclusions.

## Inference

World at War's accepted setup reduces the projected missing-setup count from five to four. This is not a new whole-library measurement. The four unavailable sources are GTA IV, LittleBigPlanet, Yakuza: Dead Souls and GTA V. A reduced selection must list and explicitly exclude them; its remaining-trophy total must be measured afresh.

## Unknown / next test

The v0.0.62 physical UI, the new skip-list controller flow, a completed whole-library award run and real HEN operation remain unverified. No automatic PSN synchronization is included.

Next: while signed out of PSN, review the missing-setup exclusions and run only the registered-games readiness check. Stop at its result before preparation or awards. Confirm included/excluded counts and preservation of saved state before progressing.

## Evidence and release files

| Package | SHA-256 |
| --- | --- |
| CFW | c3dfe68c25b6e2a296905dbf39095cc448e6429b56076401536095c4c85313a4 |
| HEN | 46e8c573ea58a4b5688e2227f41d267fc72d66253e9ed50dd274eb0897a87ce1 |

Each package is 3,560,336 bytes. App version is 01.62; CFW title ID is TROPH2026, HEN title ID is TROPHHEN1. These locally built binaries are distinct from a public-branch CI build. Historical [UI screenshot](assets/ui-v036/trophies.png) is v0.0.36, not a current hardware capture.
