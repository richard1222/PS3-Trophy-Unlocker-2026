![Platform](https://img.shields.io/badge/Platform-PlayStation_3-blue)
![Target](https://img.shields.io/badge/Target-Evilnat_4.93_CFW-orange)
![Status](https://img.shields.io/badge/Status-In_Development-yellow)

# PS3 Trophy Unlocker 2026

A project by **Richard1222/Saber** to browse and unlock selected PlayStation 3 trophies on Evilnat 4.93 CFW.

> **Latest progress:** [v0.0.10 development update](releases/v0.0.10/README.md). Browsing, backup creation and a scratch recovery test have console evidence. **A working trophy unlocker is not released.**

Updated October 23, 2026.

> **October 23, 2026 engineering update:** A private, mock-tested single-trophy API bridge and a new host-only read-only state-comparison module have been developed. The private review workflow passed for the comparison module. The native bridge has **not** been invoked successfully against a verified live trophy context, and no real trophy unlock is verified. Research identified that context ownership/registration must be validated before any write-capable build. No new public PKG or working unlocker is being claimed.

## Current progress

| Feature | Evidence |
| --- | --- |
| Browse trophy sets and individual trophies | Tested on one PS3 |
| Create a complete-set backup | Console result and file-integrity checks passed |
| Backup persistence | One owner-reported normal restart followed by successful integrity checks |
| Recover into a new scratch folder | Console result confirmed; 83 copied files / 3,672,696 bytes passed separate integrity checks |
| Compare that scratch copy against backup and live originals | Follow-up interrupted by FTP timeouts; pending |
| Unlock a selected trophy | In development; no verified unlock |
| Restore to a live trophy folder | Not implemented or tested |
| PSN synchronization | Not implemented or verified |

These are results for the tested console and trophy sets, not universal compatibility claims.

## Releases

[v0.0.10 development progress](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases/tag/v0.0.10-dev-preview-20261023) records the latest verified milestone. This public release contains a progress report and public source snapshot, with no installable PKG attached.

Earlier development notes remain in [releases/](releases/). They describe their own point in development and may predate newer results.

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
