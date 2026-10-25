![Platform](https://img.shields.io/badge/Platform-PlayStation_3-blue)
![Target](https://img.shields.io/badge/Target-Evilnat_4.93_CFW-orange)
![Status](https://img.shields.io/badge/Status-In_Development-yellow)

# PS3 Trophy Unlocker 2026

A project by **Richard1222/Saber** to browse and unlock selected PlayStation 3 trophies on Evilnat 4.93 CFW.

> **Latest progress (October 25, 2026):** The selected Persona 5 trophy remains unlocked after an owner-confirmed normal restart; all 57 captured files match the preserved post-error copy. Persona 5 loads normally while signed out. **PSN trophy synchronization failed with `80022D1E`; PSN acceptance is unverified.** See the [sync-error investigation](docs/PSN-SYNC-ERROR-2026-10-25.md). Public release remains [v0.0.10 development preview](releases/v0.0.10/README.md).

Updated October 25, 2026.

> **Earlier October 25 milestone:** First-status cancellation passed on the owner's PS3 with unchanged trophy files. The later completed-registration result above has different preservation findings. See the [historical cancellation result](docs/HARDWARE-CANCELLATION-2026-10-25.md).

## Current progress

| Feature | Evidence |
| --- | --- |
| Browse trophy sets and individual trophies | Tested on one PS3 |
| Create a complete-set backup | Console result and file-integrity checks passed |
| Backup persistence | One owner-reported normal restart followed by successful integrity checks |
| Recover into a new scratch folder | Console result confirmed; 83 copied files / 3,672,696 bytes passed separate integrity checks |
| Compare that scratch copy against backup and live originals | Follow-up interrupted by FTP timeouts; pending |
| App-owned read-only context setup/cleanup | v0.0.22 hardware diagnostic passed on one PS3 (18 report fields) |
| App-owned writable context setup/cleanup | v0.0.27 hardware report, installed payload and four-file comparison passed |
| Guarded native trophy-state query | v0.0.25 two-cycle hardware report, installed payload and independent four-file comparison passed |
| Registered-context trophy queries | Native selected-trophy details and 49-trophy flags passed; three-file preservation failure retained |
| Unlock a selected trophy | One native Persona 5 award verified by API readbacks and independent saved-file parsing |
| Unlock persistence | Local award retained after an owner-confirmed normal restart; all 57 files match post-error backup |
| Restore to a live trophy folder | Not implemented or tested |
| PSN synchronization | Owner-triggered sync failed with 80022D1E; server acceptance unverified |

These are results for the tested console and trophy sets, not universal compatibility claims.

## Releases

[v0.0.10 development progress](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases/tag/v0.0.10-dev-preview-20261023) records an earlier public development milestone. This public release contains a progress report and public source snapshot, with no installable PKG attached.

Earlier development notes remain in [releases/](releases/). They describe their own point in development and may predate newer results.

## Development milestone (October 25, 2026)

- **Proven:** v0.0.27 created and cleaned up a writable-capable context and handle on one PS3, with no retained resources. Installed payload and independent four-file comparisons passed.
- **Proven:** v0.0.25 previously completed two read-only query/cleanup cycles, each returning49 trophies. The selected trophy remains locked in the unchanged record.
- **Proven:** registered-context queries identified the selected trophy and returned 49 locked trophies on the owner's PS3. Cleanup succeeded; three of 57 collected files changed.
- **Inference:** the query result supports a bounded single-trophy hardware test; it does not prove unlocking or data integrity.
- **Proven:** exactly one selected trophy unlocked, its timestamps were saved, and the other 48 decoded trophy records were unchanged in two complete collections. Cleanup and XMB return passed.
- **Proven:** the local award survived an owner-confirmed normal restart. Persona 5 loads normally while signed out.
- **Unknown:** sync-error cause, independent protection-file integrity, general callback lifetime, PSN acceptance and HEN compatibility.
- **Next:** investigate the owner-reported synchronization error while retaining all backups. The completed award test remains disabled.

All public milestones remain development previews. The measured single-trophy result is not a general compatibility claim or a new end-user release.

## Roadmap

1. Extend the verified single-trophy result with identity, recovery and interruption checks.
2. Test identity, recovery, interrupted operations and untouched trophies.
3. Confirm local display and persistence on real hardware.
4. Evaluate online-only trophies and PSN synchronization separately.

A local state change alone will not be presented as proof of PSN acceptance.

## Compatibility

Initial target: **PS3 running Evilnat 4.93 CFW**. HEN, other firmware variants and broad hardware compatibility are unverified.

## Credits

Project creator: **Richard1222/Saber**.

Research builds on publicly available PlayStation homebrew documentation and developer-maintained source. Existing contributor credits and licenses in the public source remain applicable.

## License

Original project code is licensed under the [MIT License](LICENSE), copyright
2026 Richard (Richard1222/Saber). Third-party components retain their own
terms; see [Third-party notices](THIRD_PARTY_NOTICES.md) and the full notices
in [licenses/](licenses/).
