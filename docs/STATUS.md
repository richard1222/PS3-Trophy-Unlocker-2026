# Current status — November 6, 2026

**Saber PSN Unlock Tool v0.0.59 CFW is installed and its read-only library check passes on the test PS3.** Matching setup is still missing for six registered games, so whole-library preparation remains blocked.

## Proven

- Development target: Evilnat 4.93 CFW. Package app version: 01.59.
- The installed payload was independently read back and matched the validated candidate. All 38 prior startup reports were unchanged; setup and app-state directory names were preserved. This was not a fresh byte-for-byte archive of every setup and checkpoint.
- The owner completed navigation, exclusion review and registered-games-only preflight in v0.0.59.
- The latest report records 25 registered sets, 1,269 remaining trophies, 15 checked matching setups for unfinished sets and six missing setups.
- Fifteen uninitialized installed digital sets were explicitly excluded. No preparation, award or PSN sync was started by that check.
- Missing setup titles: GTA IV, LittleBigPlanet, Call of Duty: World at War, Yakuza: Dead Souls, Grand Theft Auto V and Super Heroine Chronicle.
- 75 unique local methods passed. The six-method scope rerun is included in the library group and is not counted twice.
- Both normal CFW/HEN package payloads reached startup/discovery in disconnected RPCS3 4.93 with authored trophy fixtures unchanged and zero trophy API calls.
- Earlier owner-confirmed PSN results remain selected-game observations. See the [historical owner-confirmed result](OWNER-CONFIRMED-PSN-2026-10-25.md).

## What changed in v0.0.59

Whole-library preparation already compared canonical records before and after native registration. The same comparison now runs for direct, individual-review and per-game batch sessions before a new review or award.

Changed definitions, flags or timestamps stop the session. Raw container rewrites may still be accepted when the records themselves are unchanged. A failed or ambiguous action is not retried automatically.

Five adversarial test scenarios cover registration clearing an earned trophy in direct, individual-review, preparation and per-game batch flows, plus changing an already-earned timestamp. They verify zero review/award activity and clean teardown.

## Inference and unknown

Two normal game launches were observed with temporary local earned-count changes. Owner-controlled normal PSN synchronization restored their earlier earned records, and those records were independently rechecked. The cause of those initialization changes has not been established.

The new app guard detects registration changes and stops subsequent app actions. It does not prevent external game writes or provide an automatic repair. Its failure behavior has local authored coverage; it has not been deliberately triggered on real hardware.

The HEN candidate is separately labeled and uses a separate app identity. Real HEN hardware, complete whole-library awards and universal game compatibility remain unverified.

The emulator UI was stopped by the host after 20 seconds. That check does not establish controller input, clean guest exit, native registration, trophy awards or PSN behavior.

## Next

Complete the six missing matching setups, investigate registration stability, and retain separate native-preparation and final-award reviews. Whole-library completion must be established through actual saved-state evidence, not inferred from a readiness screen.

The current candidate remains local. This documentation update adds no release, tag, image, package, ZIP or raw console files. The public branch's historical build workflows are not exact-commit proof of the local v0.0.59 candidate.

## Evidence scope

The current status summarizes local candidate receipts and the owner-completed hardware report. No new hardware screenshot was captured for this check. The README retains an existing v0.0.36 UI image, clearly labeled historical. Earlier pages, including [October 29 progress](PROGRESS-2026-10-29.md), describe their original dates and are superseded by this status.
