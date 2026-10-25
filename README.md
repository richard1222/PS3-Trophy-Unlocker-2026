![Platform](https://img.shields.io/badge/Platform-PlayStation_3-blue)
![Target](https://img.shields.io/badge/Target-Evilnat_4.93_CFW-orange)
![Status](https://img.shields.io/badge/Status-In_Development-yellow)

# PS3 Trophy Unlocker 2026

A project by **Richard1222/Saber** to browse and unlock selected PlayStation 3 trophies on Evilnat 4.93 CFW.

> **Latest progress (October 25, 2026):** Private development has reached **v0.0.23** with guarded, read-only native trophy-state diagnostics. A **v0.0.22 setup-only diagnostic passed on one real PS3** (18 verified report fields). The v0.0.23 package has been staged for an owner-run hardware query, but **that query has not yet been verified on hardware**. No working trophy unlocker or PSN sync is released. Public release status remains [v0.0.10 development preview](releases/v0.0.10/README.md).

Updated October 25, 2026.

> **October 25, 2026 engineering update:** Separate app-owned trophy-context setup and cleanup have been verified through a read-only hardware diagnostic. Offline tests of the subsequent guarded state query have passed, and a private package is staged for hardware verification. These tests do **not** establish safe write-capable registration, real trophy unlocking, or PSN acceptance. The separate Persona 5 game-owned context lifetime and queue exclusion remain unresolved. No private samples, logs, proprietary data, or unverified PKGs are published.

## Current progress

| Feature | Evidence |
| --- | --- |
| Browse trophy sets and individual trophies | Tested on one PS3 |
| Create a complete-set backup | Console result and file-integrity checks passed |
| Backup persistence | One owner-reported normal restart followed by successful integrity checks |
| Recover into a new scratch folder | Console result confirmed; 83 copied files / 3,672,696 bytes passed separate integrity checks |
| Compare that scratch copy against backup and live originals | Follow-up interrupted by FTP timeouts; pending |
| App-owned read-only context setup/cleanup | v0.0.22 hardware diagnostic passed on one PS3 (18 report fields) |
| Guarded native trophy-state query | v0.0.23 offline tested; owner hardware test pending |
| Unlock a selected trophy | In development; no verified unlock |
| Restore to a live trophy folder | Not implemented or tested |
| PSN synchronization | Not implemented or verified |

These are results for the tested console and trophy sets, not universal compatibility claims.

## Releases

[v0.0.10 development progress](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases/tag/v0.0.10-dev-preview-20261023) records the latest verified milestone. This public release contains a progress report and public source snapshot, with no installable PKG attached.

Earlier development notes remain in [releases/](releases/). They describe their own point in development and may predate newer results.

## Development milestone (October 25, 2026)

- Private v0.0.23 source passed both reported CI workflows (512 tests, with a Windows startup-probe test class skipped).
- v0.0.22's setup-only operation was verified on the targeted PS3. The v0.0.23 trophy-state query is **not yet hardware verified**.
- Persona 5's local trophy record was observed in a locked state during bounded read-only checks. This does not identify the active PSN user or prove synchronization.
- Native context ownership and safe request exclusion are still research blockers for write-capable operations.
- All public milestones remain development previews, not a functioning unlocker.

## Roadmap

1. Validate a backend that handles exactly one selected trophy and reports its actual result.
2. Test identity, recovery, interrupted operations and untouched trophies.
3. Confirm local display and persistence on real hardware.
4. Evaluate online-only trophies and PSN synchronization separately.

A local state change alone will not be presented as proof of PSN acceptance.

## Compatibility

Initial target: **PS3 running Evilnat 4.93 CFW**. HEN, other firmware variants and broad hardware compatibility are unverified.

## Credits

Project creator: **Richard1222/Saber**.

Research builds on publicly available PlayStation homebrew documentation and developer-maintained source. Existing contributor credits and licenses in the public source remain applicable.
