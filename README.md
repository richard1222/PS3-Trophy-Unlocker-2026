![Platform](https://img.shields.io/badge/Platform-PlayStation_3-blue)
![Target](https://img.shields.io/badge/Target-Evilnat_4.93_CFW-orange)
![Status](https://img.shields.io/badge/Status-In_Development-yellow)

# PS3 Trophy Unlocker 2026

A project by **Richard1222/Saber** to browse and unlock selected PlayStation 3 trophies on Evilnat 4.93 CFW.

> **Latest progress (October 25, 2026):** v0.0.24 completed an owner-run native read-only trophy-state query on a real PS3. Its report passed verification, and independent copies of all four trophy-data files matched the documented baseline. The tested set contained 49 trophies; the selected trophy remained locked. **Trophy unlocking and PSN synchronization remain unverified.** Public release status remains [v0.0.10 development preview](releases/v0.0.10/README.md).

Updated October 25, 2026.

> **October 25, 2026 engineering update:** The v0.0.24 report shows a successful native read-only query, successful file/user guards, clean resource cleanup, and no retained context, handle or runtime. Separate file comparisons support the result. Installed-package identity could not be independently checked afterward because the diagnostic app was unavailable. See the [milestone notes](docs/READONLY-QUERY-MILESTONE-2026-10-25.md) for evidence limits and the next test.

## Current progress

| Feature | Evidence |
| --- | --- |
| Browse trophy sets and individual trophies | Tested on one PS3 |
| Create a complete-set backup | Console result and file-integrity checks passed |
| Backup persistence | One owner-reported normal restart followed by successful integrity checks |
| Recover into a new scratch folder | Console result confirmed; 83 copied files / 3,672,696 bytes passed separate integrity checks |
| Compare that scratch copy against backup and live originals | Follow-up interrupted by FTP timeouts; pending |
| App-owned read-only context setup/cleanup | v0.0.22 hardware diagnostic passed on one PS3 (18 report fields) |
| Guarded native trophy-state query | v0.0.24 owner hardware report verified; independent four-file comparison passed |
| Unlock a selected trophy | In development; no verified unlock |
| Restore to a live trophy folder | Not implemented or tested |
| PSN synchronization | Not implemented or verified |

These are results for the tested console and trophy sets, not universal compatibility claims.

## Releases

[v0.0.10 development progress](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases/tag/v0.0.10-dev-preview-20261023) records the latest verified milestone. This public release contains a progress report and public source snapshot, with no installable PKG attached.

Earlier development notes remain in [releases/](releases/). They describe their own point in development and may predate newer results.

## Development milestone (October 25, 2026)

- **Proven:** v0.0.24's owner-supplied hardware report passed verification: one native query, 49 trophies, successful cleanup, and the selected trophy still locked.
- **Proven:** independent double reads of all four trophy-data files matched the versioned baseline. Three workflows passed on the associated development commit; 29 focused checks passed locally.
- **Inference:** the app-owned read-only route can support further lifecycle testing. One successful run does not establish repeatability or write-capable behavior.
- **Unknown:** installed-package identity after the test, safe write-capable context ownership, trophy unlocking, persistence after unlocking, PSN acceptance, and HEN compatibility.
- **Next:** test two sequential read-only lifecycle cycles with file/user checks between them and explicit package identity verification.

All public milestones remain development previews. No working unlocker is released.

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
