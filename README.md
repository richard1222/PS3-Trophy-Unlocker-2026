![Platform](https://img.shields.io/badge/Platform-PlayStation_3-003791)
![Target](https://img.shields.io/badge/Target-Evilnat_4.93_CFW-orange)
![Development](https://img.shields.io/badge/Development-v0.0.59-orange)
![License](https://img.shields.io/badge/License-MIT-blue)
![Credit](https://img.shields.io/badge/Credit-Saber-brightgreen)

# Saber PSN Unlock Tool

An XMB-style PlayStation 3 homebrew project for trophy browsing, backups, individual awards and game-wide trophy review. **Credit to Richard1222 / Saber.**

**Development target: Evilnat 4.93 CFW.** Selected-game results have been tested on hardware. Unlocking every registered game in one operation remains under development. The separately labeled HEN build has not been tested on real HEN hardware.

---

## Contents

- [Current development](#current-development)
- [Features](#features)
- [Quick start](#quick-start)
- [Screenshots](#screenshots)
- [Progress chart](#progress-chart)
- [Validation](#validation)
- [Repository layout](#repository-layout)
- [Credits and license](#credits-and-license)

## Current development

**Development build: v0.0.59 · November 6, 2026.** Read the [current status](docs/STATUS.md) for the evidence and remaining work.

The CFW build is installed on the test PS3. Its navigation and registered-games-only preflight completed on hardware:

| Latest hardware check | Result |
| --- | --- |
| Registered trophy sets | 25 |
| Remaining trophies in that selection | 1,269 |
| Matching setups checked for unfinished sets | 15 |
| Matching setups still missing | **6 — preparation remains blocked** |
| Preparation or awards started by this check | None |
| Uninitialized installed digital sets | 15, explicitly reviewed and excluded from this selection |

These are counts for the current test profile, not a compatibility list or a guarantee that every installed game is ready. Excluded games receive no awards in this selection.

v0.0.59 extends the existing preparation check to every session path. If registration changes trophy definitions, earned flags or timestamps, the app stops before presenting a new review or attempting an award. Raw file rewrites are accepted only when the canonical trophy records remain unchanged.

This check detects changes made during the app's registration flow. It does not prevent a game from changing its own files, explain every trophy-initialization error, or automatically restore data.

**v0.0.59 is a local test candidate; no new release assets are being published with this documentation update.** Files in older [Releases](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases) retain their original versions and limitations.

## Features

- **XMB-style browsing:** controller navigation, game lists, trophy lists and review screens.
- **Backups:** preserve trophy state before a state-changing operation.
- **Individual trophy review:** distinguish already-earned and locked trophies before a fresh confirmation.
- **Per-game Unlock All:** reviewed batches with saved-state verification and preservation of prior earned records.
- **Registered-games-only selection:** explicitly review installed digital sets that lack local trophy initialization before excluding them.
- **Disc and digital discovery:** include eligible PS3 titles; PS1/PS2 Classics are excluded from the PS3 trophy scan.
- **Registration checks:** compare pre/post-registration records and stop when definitions, flags or timestamps change.
- **Separate HEN identity:** a clearly labeled HEN test app with storage separate from the CFW app; hardware validation is pending.

A selection or readiness check does not award trophies. Whole-library preparation and the later award confirmation are separate steps. PSN synchronization remains manual.

## Quick start

1. Read [Current status](docs/STATUS.md) and the notes for the specific build you have.
2. For an installed test build, open Saber and browse the game and trophy lists.
3. Press **Square from Games** for the All Games check.
4. Review any excluded titles with **X**, then explicitly choose **registered games only** if that is the intended selection.
5. Let the check finish. Missing matching setups block preparation; a successful check alone does not authorize awards.

Use **Circle** to go back and **Start** to exit. Keep backups and existing reports when updating an installed test build.

[Report a bug](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/issues/new) · [Historical release files](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases)

## Screenshots

![XMB-style trophy browser](docs/assets/ui-v036/trophies.png)

*Historical v0.0.36 UI image already in this repository. It illustrates the visual direction; it is not a v0.0.59 hardware capture or proof of an award.*

## Progress chart

| Workstream | State | Evidence / limit |
| --- | --- | --- |
| XMB-style browsing and controller navigation | ✅ Hardware-tested | Current CFW app reached the library review and completed preflight |
| Disc and installed digital discovery | ✅ Focused hardware checks | Current profile and explicit exclusions; universal title coverage is unproven |
| Individual / per-game trophy workflows | ✅ Selected-game results | Earlier tests and owner-confirmed sync results are title-specific |
| Current matching setups | ⏳ 6 missing | The full current selection remains blocked |
| Registration-change protection | ✅ Local regression coverage | Five adversarial scenarios stop before review/awards; native guard behavior is not yet hardware-tested |
| Whole-library award completion | ⏳ Not validated | Readiness checks are not a completed multi-game award run |
| CFW package startup and preflight | ✅ v0.0.59 hardware check | No awards were started by the latest check |
| HEN package | ⏳ Hardware untested | Package and disconnected emulator startup checks pass |
| PSN acceptance | ✅ Owner-confirmed selected results | No guarantee for other games, profiles or future server behavior |

✅ applies only to the condition stated in that row. Read the [detailed status](docs/STATUS.md) before interpreting a local test as a hardware or server result.

## Validation

The current local candidate passed **75 unique automated test methods**:

| Group | Methods |
| --- | ---: |
| Native session lifecycle | 18 |
| Per-game batch lifecycle | 13 |
| Library planning, storage and recovery | 29 |
| Report summaries | 4 |
| Digital discovery | 9 |
| UI rendering | 1 |
| UI runtime | 1 |

Native session regression suites ran with undefined-behavior checks. The new cases simulate registration clearing earned trophies or changing an earned timestamp, and verify that review/awards never begin and resources are cleaned up.

Both CFW and HEN packages passed inspection of app identities, firmware metadata, executable contents/imports, artwork and notices. Their actual extracted payloads reached startup/discovery in disconnected RPCS3 using firmware 4.93, with unchanged authored trophy fixtures and zero trophy API calls. The host stopped the interactive UI after 20 seconds; this was not an award or clean guest-exit test.

These are **local candidate results**, not a claim that this public branch's GitHub Actions built or tested the installed executable. The current documentation update does not publish that candidate's source or binaries.

## Repository layout

```text
README.md       Project overview, features and current limits
docs/           Current status, historical milestones and existing UI images
releases/       Historical release documentation and files
.github/        Existing repository workflows
LICENSE         MIT license
```

The latest status supersedes older milestone pages. Historical releases and evidence retain their original dates.

## Credits and license

**Richard1222 / Saber** — project credit, integration and testing.

This repository uses the [MIT License](LICENSE). Existing third-party notices and their original terms remain applicable. This is an independent homebrew project; no Sony or PlayStation affiliation is implied.
