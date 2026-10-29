![Platform](https://img.shields.io/badge/Platform-PlayStation_3-blue)
![Target](https://img.shields.io/badge/Target-Evilnat_4.93_CFW-orange)
![Status](https://img.shields.io/badge/Status-In_Development-yellow)

# Saber PSN Unlock Tool

A project by **Richard1222/Saber** to browse and unlock PlayStation 3 trophies. The development target is **Evilnat 4.93 CFW**.

## Current development — October 29, 2026

**Local development build: v0.0.58.** The CFW build is installed on the test console. Separately labeled CFW and HEN packages passed package inspection and disconnected RPCS3 4.93 startup checks. **Real HEN hardware and a complete multi-game award run remain unverified.**

The owner tested the new **registered games only** review. It explicitly lists uninitialized installed digital titles before the owner chooses to exclude them. That choice does not unlock trophies: matching game setup and a later award confirmation are still required.

| Current result | Evidence and limit |
| --- | --- |
| Installed digital-game discovery | Tested on the owner's CFW PS3; PS1/PS2 Classics are excluded from the PS3 digital scan |
| Registered-game selection | Console report records 38 registered trophy sets and three explicitly excluded uninitialized digital sets |
| Remaining setup work | Initial check found 26 missing setups; two additional candidates have since been staged, leaving 24 without a staged candidate. A fresh complete-library check is still pending |
| Transformers: Fall of Cybertron | Matching setup accepted by native registration; owner cancelled the fresh review; independent comparison remains 0/51 with earned flags and trophy rows unchanged |
| The Simpsons Arcade Game | Matching setup passed offline checks and was staged; native registration and awards have not been tested for this candidate |
| Local automated validation | 73 test methods passed for the v0.0.58 development build; not a claim of GitHub CI validation |
| Every registered game in one award operation | **Not yet hardware-validated** |

Development is **paused at the owner's request**. This update changes documentation only: **no new PKG/ZIP uploads, release assets, tags or releases**.

[October 29 progress and remaining work](docs/PROGRESS-2026-10-29.md)

## Verified per-game milestones

- **Call of Duty Classic:** 38/38 saved trophies independently verified on v0.0.54; the owner confirmed manual PSN synchronization, 100% and platinum.
- **Resident Evil 6:** 71/71 locally verified; the owner confirmed manual PSN synchronization.
- **Persona 5:** 49/49 locally verified. The earlier owner-confirmed PSN milestone for “Merciless Executioner” is documented separately.
- **Eiyuu Senki:** 34/34 locally verified; the owner confirmed manual PSN synchronization.

Owner-reported PSN outcomes are separate from local file verification. They do not establish universal PSN compatibility or new-build validation.

## UI

![Historical trophy-list illustration using authored example data](docs/assets/ui-v036/trophies.png)

This is an existing **historical authored UI illustration**, not a screenshot of the current exclusions review.

## Releases and compatibility

Existing downloads remain on the [releases page](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases). The previously published [v0.0.51 test release](https://github.com/richard1222/PS3-Trophy-Unlocker-2026/releases/tag/v0.0.51) is a historical development build; **it does not contain the local v0.0.58 changes**.

The earlier v0.0.51 freeze report remains preserved in the [investigation](docs/V051-SQUARE-FREEZE-2026-10-27.md). Later successful tests apply to their specific tested flows; they do not prove every freeze cause is resolved.

Target: **Evilnat 4.93 CFW**. The HEN test package has its own XMB label and storage. Its real-hardware compatibility remains pending. Game-specific setup is required and is not included in public packages.

## Next development steps

1. Validate the staged Simpsons setup without awarding trophies.
2. Collect and verify matching setup for the remaining registered games.
3. Validate complete-library preparation, cancellation and recovery before a full award test.
4. Test separately on real HEN hardware before claiming HEN compatibility.

Local saved results and manual PSN synchronization remain separate checks.

## Earlier evidence

[Persona 5 local batch](docs/PERSONA5-LOCAL-BULK-2026-10-26.md) · [Digital-game result](docs/DIGITAL-GAME-RESULT-2026-10-26.md) · [Owner-confirmed selected-trophy PSN result](docs/OWNER-CONFIRMED-PSN-2026-10-25.md) · [Earlier all-games progress](docs/UNLOCK-ALL-PROGRESS-2026-10-27.md)

## Credits and license

Project creator: **Richard1222/Saber**. Existing contributor credits and third-party licenses remain applicable.

Original project code is licensed under the [MIT License](LICENSE), copyright 2026 Richard (Richard1222/Saber). See [Third-party notices](THIRD_PARTY_NOTICES.md) and [licenses/](licenses/).
