![Platform](https://img.shields.io/badge/Platform-PlayStation_3-blue)
![Target](https://img.shields.io/badge/Target-Evilnat_4.93_CFW-orange)
![Status](https://img.shields.io/badge/Status-Research_%26_Development-yellow)
![Hardware](https://img.shields.io/badge/Console_Validation-v0.0.6_Read--Only_Tested-green)

# PS3 Trophy Unlocker 2026

> **Latest beta:** [v0.0.6 Read-Only Beta release notes](releases/v0.0.6/README.md). Browsing was owner-tested on Evilnat 4.93 CFW. **No public PKG is attached yet; trophy unlocking and PSN sync are not supported.**

A homebrew project for viewing and unlocking selected trophies on **Evilnat 4.93 custom firmware**. Development starts with trophy-format inspection, backups and a verified build environment, then moves to the on-console unlock interface.

**Current status (October 22, 2026):** v0.0.6 **read-only trophy browsing** has been owner-tested on one Evilnat 4.93 CFW PS3. No trophy-unlocking or PSN-sync capability has been verified. [Public beta release notes](releases/v0.0.6/README.md) are available; the installation PKG is not yet publicly distributed or checksum-verified for release.

Last updated: October 22, 2026.

---

## Table of contents

- [Project goal](#project-goal)
- [Current status](#current-status)
- [Planned features](#planned-features)
- [How the trophy system works](#how-the-trophy-system-works)
- [Compatibility](#compatibility)
- [Quick start](#quick-start)
- [Development roadmap](#development-roadmap)
- [Progress chart](#progress-chart)
- [Testing and evidence](#testing-and-evidence)
- [Releases](#releases)
- [Research sources](#research-sources)
- [Credits and attribution](#credits-and-attribution)

---

## Project goal

Build a PS3 application that lets the owner select a local user, browse installed trophy sets, inspect individual trophies and unlock a selected trophy with a recoverable backup.

The first target is **Evilnat 4.93 CFW**. We will verify the implementation against source and actual console results before claiming support. Firmware changes, account changes and PSN automation are outside the initial implementation.

## Current status

| Area | Current evidence |
| --- | --- |
| Repository | Project initialized; source, findings and test results will be kept here. |
| Evilnat research | Developer-maintained XAI and Cobra 4.93 source inspected. |
| Trophy data | Apollo, RPCS3, TROPHYParser and PS3 Developer Wiki references identified and cross-checked. |
| Native build | Official SDK pinned by SHA-256; CI workflow and read-only probe source added; build result pending. |
| Inspector | Implemented decoded-layout tool; seven host test methods pass across the inspector and SDK fetcher. |
| Unlock backend | Not implemented or tested yet. |
| PS3 validation | Pending; no trophies have been modified by this project. |
| Release | No package published yet. |

**Important implementation finding:** Cobra's `PS3MAPI_OPCODE_TROPHY` (`0x2253`) changes hidden-trophy display behavior. `PS3MAPI_OPCODE_CUSTOM_TROPHY_TIMESTAMP` (`0x2254`) sets a timestamp value. Neither opcode directly unlocks an arbitrary selected trophy. XAI's trophy option loads a separate `trophyUnlocker.sprx` plugin. These distinctions come from the inspected developer source, not a successful run of our own app.

## Planned features

These are development targets, not available features.

| Feature | Intended behavior | Status |
| --- | --- | --- |
| Trophy-set browser | List installed sets for an explicitly selected local user. | Planned |
| Trophy details | Show ID, grade, unlock state and available metadata. | Planned |
| Selected-trophy unlock | Update one selected trophy through a verified backend. | Research |
| Backup before changes | Preserve the complete affected trophy folder and verify the backup. | Planned |
| Restore | Restore a matching backup with identity checks. | Planned |
| Data validation | Read-only tool rejects truncated files, invalid offsets, overlaps and unsupported layouts. | Host-tested |
| Build receipts | Record source revision, build environment and package hashes. | Planned |
| Console diagnostics | Distinguish file, format, signing and runtime errors. | Planned |

## How the trophy system works

Installed trophy sets normally live under:

```text
/dev_hdd0/home/<local-user-id>/trophy/<NPWR-set-id>/
```

| File or folder | Role in the implementation |
| --- | --- |
| `TROPCONF.SFM` | Trophy definitions, identifiers and localized metadata. |
| `TROPUSR.DAT` | User trophy state and timestamps in the supported decoded layout. |
| `TROPTRNS.DAT` | Additional trophy transaction/synchronization data. |
| `PARAM.PFD` | Protection metadata that must be handled when protected files change. |
| `_TROPSYS_` | Per-user trophy installation index; distinct from an individual set. |

Apollo's actual write path decrypts transaction data, applies trophy changes, re-encrypts it and updates protection metadata. A successful implementation needs to account for that sequence; changing a displayed flag alone is not our completion criterion.

A native trophy API also has a context and registration lifecycle. RPCS3's implementation is useful behavioral evidence, but an emulator implementation alone does not prove identical retail-firmware behavior.

## Compatibility

| Environment | Status |
| --- | --- |
| Evilnat 4.93 CEX | Initial target; untested |
| Evilnat 4.93 PEX / D-PEX | Variant-specific validation pending |
| Other CFW versions | Not claimed |
| PS3 HEN | Outside the initial target |
| RPCS3 | Source reference; not a substitute for PS3 testing |
| PC inspection tools | Planned development helpers, not the final PS3 interface |

The exact firmware variant and relevant modules will be recorded with console tests. No unsupported firmware offsets will be treated as verified addresses.

## Quick start

**Available now:** read-only inspection tools, synthetic tests, and an owner-tested v0.0.6 console trophy browser (not yet publicly packaged). No functional trophy-unlocking release exists. See [tool commands and limits](docs/TOOLS.md) and [source research](docs/RESEARCH-2026-10-22.md).

```text
python -m unittest discover -s tests -v
python tools/inspect_trophy.py decoded-TROPUSR.DAT --output new-report.json
```

When a build is ready, this section will contain its exact filename, prerequisites, controller controls, backup location and first-console-test steps. Packages will be linked from [GitHub Releases](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases), with checksums and an explicit description of what they can do.

Do not supply console IDs, account credentials or private trophy backups in public issues. Test data committed to this repository will be synthetic or explicitly cleared for publication.

## Development roadmap

| Milestone | Completion condition | Status |
| --- | --- | --- |
| 1. Source and documentation research | Pin useful developer sources and separate observed facts from assumptions. | In progress |
| 2. Read-only inspection tools | Seven host test methods pass; real console data remains unverified. | Host-tested |
| 3. Reproducible PS3 build | Build with a pinned open-source SDK and record the output identity. | In progress |
| 4. Read-only console probe | Verify installed-set discovery on the target console without trophy writes. | Planned |
| 5. Backup and restore | Demonstrate complete backup verification and recovery on controlled test data. | Planned |
| 6. Unlock implementation | Implement the complete selected-trophy update, including protection handling. | Research |
| 7. Controlled PS3 test | Verify the selected change and preservation of other data using a test profile. | Pending |
| 8. Release | Test the exact packaged tools, publish checksums and verify the downloaded assets. | Pending |

## Progress chart

| Task | Result |
| --- | --- |
| Confirm target | Evilnat 4.93 CFW requested |
| Review Evilnat code | XAI plugin-loading path and Cobra trophy-related opcodes identified |
| Review existing editors | Apollo and darkautism's tools identified |
| Review API behavior | RPCS3 context, registration and unlock checks inspected |
| Inspect protected-file requirements | Transaction encryption and PFD update sequence identified |
| Build our application | Not completed |
| Unlock a trophy with our application | Not tested |
| Publish a working unlocker | Not completed |

## Testing and evidence

Each meaningful milestone will record:

- Source revision and relevant upstream revisions.
- Synthetic fixtures and reproducible test commands.
- Exact input/output hashes where bytes matter.
- Build/package verification and preservation checks.
- Console model, firmware variant and observed behavior.
- Remaining unknowns and the next test.

**No console results are currently claimed.** No account-sync or universal compatibility claim follows from a local file-editing test.

## Releases

[View releases](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases)

There are no published releases yet. Release pages will use the same evidence-focused structure as the SpongeBob Movie tools project: clear features, controls, prerequisites, known limits, source identity, checksums and downloadable assets.

An inspection-only build will be labeled **read-only**. A development probe will not be presented as a completed trophy unlocker.

## Research sources

| Primary source | Why it matters |
| --- | --- |
| [Evilnat XAI plugin](https://gitlab.com/Evilnat/xai_plugin/-/tree/b6b0cbf26af3c5d2758d028106d4f37a17ba211e) | Actual trophy-plugin loading and CFW integration. |
| [Evilnat Cobra source](https://gitlab.com/Evilnat/Cobra-PS3/-/tree/d7ba66ba32f3f4edf7728b2fe9137b8e1c93049f) | 4.93 CEX implementation and trophy-related opcode behavior. |
| [Apollo trophy editor](https://github.com/bucanero/apollo-ps3/blob/7c0686d2f242773ac3f3230b8a9c7cd2aabc52b9/source/trophy.c) | Real editor implementation and linked upstream format work. |
| [Apollo protection/update path](https://github.com/bucanero/apollo-ps3/blob/7c0686d2f242773ac3f3230b8a9c7cd2aabc52b9/source/exec_cmd.c) | Decrypt, edit, encrypt and PFD-update sequence. |
| [RPCS3 trophy API](https://github.com/RPCS3/rpcs3/blob/0dec46b12e856756ba04271207a99a1de2208720/rpcs3/Emu/Cell/Modules/sceNpTrophy.cpp) | Context lifecycle, input checks and unlock behavior; emulator reference. |
| [RPCS3 trophy layouts](https://github.com/RPCS3/rpcs3/blob/0dec46b12e856756ba04271207a99a1de2208720/rpcs3/Loader/TROPUSR.h) | Explicit big-endian table and entry layouts. |
| [PS3 Developer Wiki: trophies](https://www.psdevwiki.com/ps3/Trophy) | Filesystem and format research; uncertain or older claims require cross-checking. |
| [PS3 Developer Wiki: PARAM.PFD](https://www.psdevwiki.com/ps3/PARAM.PFD) | Protected-file metadata and update requirements. |
| [PS3TrophyIsGood](https://github.com/darkautism/PS3TrophyIsGood) / [TROPHYParser](https://github.com/darkautism/TROPHYParser) | Earlier editor/parser implementations and known edge cases. |
| [PSL1GHT](https://github.com/ps3dev/PSL1GHT) / [ps3dev](https://github.com/ps3dev/ps3dev) | Open-source application SDK and build environment. |

## Credits and attribution

This is an independent community project. Research and code from **Evilnat, mysis, bucanero, darkautism, RPCS3 contributors and ps3dev contributors** are credited to their respective authors. Other contributors will be credited as their work is used.

Links do not imply endorsement or collaboration. Referenced projects retain their own licenses; reused source will retain the required notices and license terms. No Sony SDK, firmware modules, keys, account credentials or private console data will be packaged in releases.

PlayStation and related names belong to their respective rights holders.
