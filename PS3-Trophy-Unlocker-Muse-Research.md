# PS3 Trophy Unlocker 2026 — Muse research: sceNpTrophy contexts and validating a running game's trophy state

- Date: October 23, 2026. Author: Muse (mobile), for Richard1222/Saber's private project `PS3-Trophy-Unlocker-2026`.
- Target: Persona 5 (BLUS31604), trophy ID 11, "Merciless Executioner".
- Console context (owner-supplied): PS3 on Evilnat 4.93 D-PEX with webMAN MOD and PS3MAPI.
- Scope honored throughout: **no trophy files were modified, no live game memory was patched, and no PSN synchronization was attempted.** Console access for this research was read-only: FTP directory listings and one read of an installed trophy *configuration* file (TROPCONF.SFM). No user trophy data file (TROPUSR.DAT) was read or touched.
- Labeling: **[VERIFIED]** = established from primary source read for this document (source pinned in §11). **[THEORY]** = reasoned inference or untested claim, with the experiment that would settle it. This continues the labeling style of `docs/RESEARCH-2026-10-22.md`, which this document extends rather than replaces.

## 1. Executive summary

The project's native backend passes its tests but has never been shown a *verified running-game trophy context*. This research establishes what that phrase means mechanically, and how to prove one exists without changing anything:

1. A trophy "context" is a per-process object created through the `sceNpTrophy*` API lifecycle (§3). It is created **by the game process itself**, for its own trophy set, and there is **no API to open, inspect, or reuse another process's context or handle** — verified from the complete API inventory. Any design that assumes cross-process context access is dead on arrival.
2. The target is fully identified from the console's own installed configuration (§2): Persona 5's set is **NPWR11866_00**, and **SFM trophy id 11 is exactly "Merciless Executioner"** (bronze, not hidden). The often-guessed NPWR11779 is **not** this game's set and is absent from the console.
3. A running game's context can be validated read-only, in increasing strength: process presence via PS3MAPI (§7 V1), filesystem side-effects of registration (V3, already observed on this console today), a read-only memory scan for the set's communication ID / signature inside the game process (V2), and strongest of all, a homebrew probe that creates its **own read-only context** for the same set and reads state through the native API (V4). The API explicitly supports a read-only context option, and every gate in the lifecycle reports a distinct error code (§4), so a failed experiment names its own blocker.
4. One byte-level unknown gates the native route: the first 4 bytes of the runtime communication signature (§5). It is resolvable two ways, both read-only (V2 memory read of the running game, or the V4 probe's result code).
5. Recommended implementation (§8): extend the existing read-only probe with a native-context stage that can only read. Keep unlock attempts, file writes, and any network sync out of it. Test in the staged gates of §9.

## 2. Target card — Persona 5 (BLUS31604) **[VERIFIED from the console's installed TROPCONF.SFM, read read-only 2026-10-23]**

| Fact | Value |
| --- | --- |
| Trophy set (NP communication ID) | **NPWR11866_00** |
| Set folder on console | `/dev_hdd0/home/00000001/trophy/NPWR11866_00/` — present, with TROPCONF.SFM, PARAM.PFD and 49 trophy icons (TROP000–TROP048) |
| Title name in config | "Persona 5" / "Persona 5 Trophy Pack" |
| Trophyset version | 01.00 |
| Platform tag in config | `ps3,ps4` (one shared set serves both versions) |
| Trophy count | 49 (matches public listings: 40 bronze, 5 silver, 3 gold, 1 platinum) |
| **Trophy id 11** | **"Merciless Executioner" — detail "Perform 50 Persona fusions." — type B (bronze), hidden = no, pid = 000** |
| Platinum | id 0, "Legendary Phantom Thief", type P, pid = -1; every other trophy carries pid = 000 (the platinum link) |
| Set activity on this console | Set folder and PARAM.PFD timestamps updated 2026-10-23 (the game has run and registered/saved on this console today) |
| Game data folder | `/dev_hdd0/game/BLUS31604/` does **not** exist — consistent with the disc release; the set's config reached the user folder from the game's TROPDIR at registration time |

Consequences:

- The project's "trophy ID 11" is the SFM id, which is also the `s32 trophyId` the API takes (RPCS3 validates `0 <= trophyId < trophyCount`, §3). No index translation is needed for this target, and the earlier project warning — map by explicit IDs, not by row order in public lists — is confirmed necessary: public list orderings (popularity/date) do not match SFM order.
- NPWR11779_00, a plausible-looking guess for this game, is **not installed** on the console and is not Persona 5's set. Do not use it anywhere.
- Because the SFM `id` attributes run "000".."048" in file order and the platinum is id 0, "unlock everything except platinum" style logic must special-case id 0 — `sceNpTrophyUnlockTrophy` refuses platinum IDs outright (§3).

## 3. The sceNpTrophy lifecycle **[VERIFIED from RPCS3 source]**

RPCS3's `sceNpTrophy.cpp` is the most complete public model of the API (the 2026-10-22 research already pinned a revision for CreateContext/UnlockTrophy validation; this document re-read current master for the full lifecycle, §11). Emulator code proves API shape and semantics; where retail firmware might differ, §10 says so.

### 3.1 API inventory (22 exported functions)

`sceNpTrophyInit`, `sceNpTrophyTerm`, `sceNpTrophyCreateHandle`, `sceNpTrophyDestroyHandle`, `sceNpTrophyAbortHandle`, `sceNpTrophyCreateContext`, `sceNpTrophyDestroyContext`, `sceNpTrophyRegisterContext`, `sceNpTrophyGetRequiredDiskSpace`, `sceNpTrophyGetGameInfo`, `sceNpTrophyGetGameDetails`, `sceNpTrophyGetGameProgress`, `sceNpTrophyGetTrophyInfo`, `sceNpTrophyGetTrophyDetails`, `sceNpTrophyGetTrophyUnlockState`, `sceNpTrophyGetLatestTrophies`, `sceNpTrophyGetUserInfo`, `sceNpTrophyGetGameIcon`, `sceNpTrophyGetTrophyIcon`, `sceNpTrophySetSoundLevel`, `sceNpTrophyUnlockTrophy`, `sceNpTrophyNetworkSync`.

Two structural facts follow from the inventory alone:

- **There is no cross-process function.** No "get context of title X", no handle lookup, no enumeration of other processes' contexts. A context is an opaque `u32` meaningful only inside the process (and the firmware service state) that created it.
- `sceNpTrophyNetworkSync` is the only sync entry point. It is out of scope for this project phase and is listed here only so its absence from the probe is a conscious decision.

### 3.2 Lifecycle and gates, in call order

1. **`sceNpTrophyInit(pool, poolSize, containerId, options)`** — must precede everything; calls before it fail with `SCE_NP_TROPHY_ERROR_NOT_INITIALIZED (0x80022902)`. A second init returns `ALREADY_INITIALIZED (0x80022901)`.
2. **`sceNpTrophyCreateHandle(&handle)`** — a handle is the work token paired with a context in every later call.
3. **`sceNpTrophyCreateContext(&context, &commId, &commSign, options)`** — binds a communication ID + signature to a fresh context ID.
   - `commId` is a `SceNpCommunicationId`: 9-char prefix + numeric suffix; the set name is composed as `"%s_%02d"` — e.g. `NPWR11866` + 0 → the folder name `NPWR11866_00`. Suffix > 99 → `INVALID_NP_COMM_ID (0x80022918)`.
   - `commSign` is a 160-byte `SceNpCommunicationSignature`. Format checks applied before the context exists: bytes [0..3] must equal the big-endian magic **0xB9DDE13B**, bytes [4..5] must be version **0x0100**, bytes [6..11] must be zero — any mismatch → `INVALID_NP_COMM_ID`. (See §5 for how this relates to the signature stored in TROPCONF.SFM.)
   - `options`: 0, or `SCE_NP_TROPHY_OPTIONS_CREATE_CONTEXT_READ_ONLY (1)`. Anything larger → `NOT_SUPPORTED (0x80022903)`. **The read-only option is the load-bearing fact for §7 V4 / §8.**
4. **`sceNpTrophyRegisterContext(context, handle, statusCb, arg, options)`** — the step that turns a context into a *registered* one:
   - Fails with `CONTEXT_NOT_REGISTERED`-family errors if the context/handle pair is not valid, `INVALID_ARGUMENT` if no status callback is supplied.
   - In the RPCS3 model it opens `TROPHY.TRP` from **the running title's own TROPDIR** (`<title dir>/TROPDIR/<SET>/TROPHY.TRP`, falling back to `/dev_bdvd/PS3_GAME/TROPDIR/<SET>/TROPHY.TRP` for disc titles); missing config → `CONF_DOES_NOT_EXIST (0x8002291B)`. Which locations retail firmware searches for a *foreign* set is the open question of §10 — it is the single most important thing the §9 gates test.
   - It invokes the status callback with the install status (`NOT_INSTALLED (1)` / `INSTALLED (3)`, then `PROCESSING_SETUP/PROGRESS/FINALIZE/COMPLETE (5..8)`); a callback returning < 0 aborts with `PROCESSING_ABORTED (0x8002290F)`.
   - It installs the set into `/dev_hdd0/home/<user>/trophy/<SET>/` (creating TROPCONF.SFM etc.) and loads the user's TROPUSR.DAT against the config. The context is now backed by live trophy data.
5. **Reads** — `sceNpTrophyGetTrophyUnlockState(context, handle, &flags, &count)` returns a 128-bit flag array + count; for a **read-only context** RPCS3 loads TROPUSR.DAT + TROPCONF.SFM fresh from the user folder and answers without any write path. `GetGameInfo`/`GetGameDetails`/`GetTrophyInfo` similarly read registered state. All fail with `CONTEXT_NOT_REGISTERED (0x80022904)` if step 4 did not complete — which makes them perfect probes: their success *is* the registration proof.
6. **`sceNpTrophyUnlockTrophy(context, handle, trophyId, &platinumId)`** — gates, in order: initialized → context/handle valid → context registered (else 0x80022904) → `0 <= trophyId < count` (else `INVALID_TROPHY_ID 0x80022920`) → grade is not platinum (else `CANNOT_UNLOCK_PLATINUM 0x80022914`) → not already unlocked (else `ALREADY_UNLOCKED 0x80022915`). On success it stamps the current RTC tick, saves TROPUSR.DAT immediately, and auto-unlocks the platinum (returning its id) when this trophy completes the set. **Not called anywhere in the recommended probe** (§8); documented so the backend's eventual unlock milestone inherits the exact gate list.
7. Teardown: `sceNpTrophyDestroyContext`, `sceNpTrophyDestroyHandle`, `sceNpTrophyTerm`.

### 3.3 What "a verified running-game trophy context" therefore means

A context is verified **for a process** when that process has completed steps 1–4 and a step-5 read returns data consistent with the installed set (count = 49 for the target; flag state matching the TROPUSR the file backend parses). For the *game's own* context, an outside observer cannot call step 5 — the evidence routes of §7 are how an observer gains equivalent confidence.

## 4. Error codes as validation signals **[VERIFIED — full enum in `sceNpTrophy.h`]**

The probe should log raw codes; these are the ones with diagnostic value for context work:

| Code | Name | Meaning for the probe |
| --- | --- | --- |
| 0x80022902 | NOT_INITIALIZED | Init skipped or already termed |
| 0x80022904 | CONTEXT_NOT_REGISTERED | RegisterContext did not complete — the core negative result |
| 0x80022918 | INVALID_NP_COMM_ID | commId malformed **or signature format check failed** (§5) |
| 0x80022919 | UNKNOWN_NP_COMM_ID | Firmware does not recognize the set |
| 0x8002291B | CONF_DOES_NOT_EXIST | Trophy config (TRP) not found where the firmware looked |
| 0x8002291E | BROKEN_DATA | Installed set data failed parsing |
| 0x8002291F | VERIFICATION_FAILURE | Signature verification failed against the config |
| 0x80022920 | INVALID_TROPHY_ID | trophyId out of range for the registered set |
| 0x80022915 | ALREADY_UNLOCKED | (Unlock path only) target already unlocked |
| 0x80022914 | CANNOT_UNLOCK_PLATINUM | (Unlock path only) platinum IDs are refused by design |

## 5. The signature question **[VERIFIED bytes; THEORY construction]**

- **[VERIFIED]** The target's installed TROPCONF.SFM embeds `<!--Sce-Np-Trophy-Signature: …-->`: a **160-byte** value (320 hex chars), exactly the size of `SceNpCommunicationSignature`. Its layout matches the runtime struct's checked fields at the same offsets: bytes [4..5] = 0x0100 (version), bytes [6..11] = 0 (padding).
- **[VERIFIED]** The one divergence: SFM bytes [0..3] are a per-set value (`4c39b98c` for NPWR11866_00), whereas RPCS3's CreateContext requires the constant magic 0xB9DDE13B at [0..3] of the struct a caller passes.
- **[THEORY]** The runtime signature is therefore most plausibly constructed as `magic(0xB9DDE13B) ‖ SFM bytes [4..160)` — i.e., the SFM stores the set-specific material and the SDK-side struct prepends the fixed magic in place of the SFM's first word. Alternative: retail firmware accepts the SFM value verbatim and RPCS3's magic models an SDK-side wrapper. Either way, exactly 4 bytes are in doubt and 156 are pinned by the console's own config file.
- **Resolution routes (both read-only):** (a) the §7 V2 memory read finds the signature *as the running game itself holds it*, settling the construction directly; (b) the §7 V4 probe tries the candidate construction and the result code discriminates: success or 0x8002291F/0x80022918 localizes the fault. Do not guess further in code — make the 4 bytes a named, logged parameter of the probe.

## 6. Homebrew reality checks **[VERIFIED by source search, 2026-10-23]**

- **PSL1GHT ships no `sceNpTrophy` declarations** (code search over `ps3dev/PSL1GHT`: zero hits for the trophy API). A homebrew using this API declares the functions/structs itself (from SDK documentation) and links the system module stubs; there is no PSL1GHT sample to copy. Consequence: the probe's header is part of the deliverable and should be checked against the RPCS3 header layouts in §11.
- **Apollo Save Tool uses zero native trophy API calls** (code search over `bucanero/apollo-ps3`: zero hits for `sceNpTrophy`). Apollo is purely the file route already documented in `docs/RESEARCH-2026-10-22.md` (decrypt transaction data → edit → re-encrypt → PFD update). It therefore offers no precedent — positive or negative — for the native-context route. The native route's reference implementations are: real games, RPCS3's model, and the still-unexamined `trophyUnlocker.sprx` that XAI loads (closed behavior; the 2026-10-22 findings stand).
- Cobra's trophy-related opcodes (0x2253/0x2254) remain non-unlock facilities per the earlier research; nothing found since changes that.

## 7. Validating a running game's trophy context — methods, ranked

All methods below are read-only. PS3MAPI write operations (`MEMORY SET`, LV2 poke) exist in the same servers and are **excluded by scope** — do not use them for this project phase.

**V1 — Process presence via PS3MAPI [VERIFIED capability].** webMAN MOD embeds the PS3MAPI server (native protocol on port 7887, plus HTTP equivalents in the web interface, incl. a process/memory debugger view). Enumerating processes and identifying the game process (title BLUS31604) proves the game is running. It proves nothing about trophies by itself — it is the precondition check for V2 and the timing reference for V3.

**V2 — Read the game process's memory for the set's context parameters [mechanism VERIFIED; layout THEORY].** PS3MAPI `MEMORY GET` (and webMAN's HTTP peek) can read process memory without writing. A game that uses the trophy API necessarily holds, in its own memory, the `SceNpCommunicationId` ("NPWR11866") and the 160-byte `SceNpCommunicationSignature` it passed to CreateContext — these are inputs the game binary/SDK runtime keeps. Scanning the game process's readable ranges for the ASCII commId and for the signature's known 156 bytes (§5) yields two results at once: positive evidence the running game is bound to NPWR11866_00, and the ground-truth first 4 bytes of the runtime signature. **[THEORY]** until executed: SDK-internal layout varies, the strings may sit in a loaded PRX data segment rather than the main ELF image, and a scan must be bounded (read in pages, stop at first confirmed hit) to stay a lightweight observer. A negative scan is inconclusive, not a disproof.

**V3 — Filesystem side-effects of registration [VERIFIED on this console].** RegisterContext installs/updates `/dev_hdd0/home/<user>/trophy/<SET>/`, and unlocks/saves rewrite its files. Observed today: NPWR11866_00's folder and PARAM.PFD carry today's timestamps after the game ran. Polling those timestamps (the existing probe already enumerates these folders read-only) brackets *when* the game's context lifecycle ran. It does not prove the context is alive *now* — pair with V1.

**V4 — Homebrew read-only context for the same set [API support VERIFIED; retail acceptance = the experiment].** A probe process performs §3 steps 1–4 with `CREATE_CONTEXT_READ_ONLY`, then `GetTrophyUnlockState` + `GetGameInfo`, then clean teardown. Success proves the strongest available statement: *retail firmware on this console accepts a native context for NPWR11866_00 from a non-game process and serves its state* — the exact capability the unlock milestone would build on. Failure returns one §4 code that names the gate (most informative failures: 0x8002291B → firmware would not supply a foreign set's TRP; 0x8002291F/0x80022918 → signature construction wrong, see §5). The probe must not call `UnlockTrophy` or `NetworkSync`; a read-only context has no write path in the RPCS3 model, and the probe's own code should not link the two write-capable calls at all.

**Non-methods (do not design around them):** attaching to the game's existing context/handle from another process (no such API, §3.1); inferring context state from the XMB trophy viewer (it reads files, not the live context — equivalent to V3 at best); Cobra opcodes (§6).

## 8. Recommended safe implementation

Extend the existing read-only discovery probe (`source/probe.c`, v0.0.2) into a **v0.0.3 context probe**, keeping its never-write-to-trophy-folders discipline:

1. **Stage A (existing):** enumerate users/sets; for the target set, parse TROPCONF.SFM (name, count, per-id name/type/hidden/pid) and the TROPUSR header as today. Extract the SFM signature into a 160-byte buffer; build the candidate runtime signature per §5 with the first word as a **named switch** (`SIG_WORD_SFM` vs `SIG_WORD_MAGIC`), defaulting to MAGIC, logged in the report.
2. **Stage B (new, native read-only):** `sceNpTrophyInit` → `CreateHandle` → `CreateContext(commId = {"NPWR11866", 0}, candidateSign, READ_ONLY)` → `RegisterContext` with a logging status callback → `GetGameInfo` + `GetTrophyUnlockState` → compare count and flags against Stage A's file parse → `DestroyContext/DestroyHandle/Term`. Log **every** return code verbatim (§4 table in the report header). The translation unit must not reference `sceNpTrophyUnlockTrophy` or `sceNpTrophyNetworkSync` — enforce by a build-time symbol check on the produced ELF (grep its import table; fail the build if either name appears).
3. **Stage C (optional, external):** while Persona 5 is running, repeat V3 timestamp capture before/after the game's own registration window, and — only if V2 is attempted — run the bounded MAPI memory scan from the PC side (no console-side code, no writes; the scan tool reads via the MAPI GET verb and stores nothing on the console).
4. **Reporting:** extend the probe's diagnostic file with a `context_probe` section: stage reached, each call's code, count/flags comparison verdict, signature-word mode used. Keep the report in the probe's own directories, as today.

## 9. Testing strategy (staged gates)

- **G0 — Host/unit.** The backend's existing suite (owner-reported 60 tests) plus new fixtures: synthetic TROPCONF.SFM parse (use the target's real layout shape: 49 trophies, platinum id 0, pid links), signature-buffer construction (156 pinned bytes + switchable word), and the ELF import-table check from §8. No console involved.
- **G1 — RPCS3 rehearsal.** Run the probe as a homebrew ELF under RPCS3 with a scratch game's trophy set installed. RPCS3 implements the exact gate order of §3.2, so the probe's logging and teardown can be debugged where iteration is free. Expectation: full success is achievable here even before the retail questions (§10) are answered — treat G1 success as *necessary, not sufficient*.
- **G2 — Console, scratch set, read-only.** On the target console, point Stage B at a set belonging to a throwaway title (or a second local user) — never the live Persona 5 set first. Success = registration completes and the read state matches the file parse. Any §4 failure stops the line and gets recorded in `knowledge/negative/` per project habit.
- **G3 — Console, target set, read-only.** Stage B against NPWR11866_00 while the game is **not** running. Success = count 49, flags equal to the backend's TROPUSR parse, clean teardown, and the set's folder byte-identical before/after (hash the folder listing + file hashes in the report — the project's backup tooling already does this class of check).
- **G4 — Cross-validation with the game running.** Launch Persona 5 (V1 confirms the process), let it reach gameplay (its context registers at boot), then: V3 timestamps, V2 scan (if attempted), and a repeat of G3's probe reads. The probe's own context and the game's context are independent objects; agreement of their read state is the deliverable called "verified running-game trophy context" for this phase.
- **Explicitly deferred:** any unlock call, any trophy-file write, any `NetworkSync`. Those belong to later milestones gated on the project's backup/restore evidence, and a local change must still never be reported as PSN acceptance (per `docs/PROJECT-GOALS.md`).

## 10. Risks and open unknowns

- **[THEORY] Retail RegisterContext config source.** RPCS3 reads the TRP from the *calling title's* TROPDIR. If retail firmware does the same, a standalone probe cannot register a foreign set unless the config is supplied another way (e.g., firmware falls back to the already-installed user-folder copy — plausible, since that copy exists precisely because the game registered once). G2/G3 exist to answer this; 0x8002291B is the tell.
- **[THEORY] Signature first word** (§5). Two candidate constructions; the probe tests the MAGIC one first because RPCS3 demands it.
- **Firmware variant drift.** The owner describes the console as Evilnat 4.93 **D-PEX**; earlier project notes target CEX. Module identities and behavior for trophy services may differ by variant — record the variant in every console report (the probe report header should carry it) and do not generalize results across variants.
- **Process binding on retail.** RPCS3 models context state per emulated process. On retail, parts of the trophy service live in system software; whether a second process registering the *same* set concurrently with the game (G4) is accepted, or refused (e.g., a LOCKED-class error, 0x80022912), is untested. G4 is read-only either way; a refusal is a finding, not a failure of method.
- **MAPI scan sensitivity (V2).** Reading game memory is read-only but still touches a running game; keep scans paged and infrequent, and never on an online session.

## 11. Sources (primary first)

- RPCS3, `rpcs3/Emu/Cell/Modules/sceNpTrophy.cpp` and `sceNpTrophy.h` (master, read 2026-10-23; project-pinned earlier revision 0dec46b12e856756ba04271207a99a1de2208720 in `docs/RESEARCH-2026-10-22.md`): lifecycle, gate order, error enum, read-only option, RegisterContext TRP lookup and install paths, UnlockTrophy gates (§§3–5).
- The target console's installed `/dev_hdd0/home/00000001/trophy/NPWR11866_00/TROPCONF.SFM` (read read-only via FTP through the owner's PC bridge, 2026-10-23): set identity, 49-trophy table incl. id 11, signature bytes (§§2, 5). Directory listings of `/dev_hdd0/home/00000001/trophy/` and `/dev_hdd0/game/` from the same session (§§2, 7 V3).
- GitHub code search, 2026-10-23: `ps3dev/PSL1GHT` (no trophy API), `bucanero/apollo-ps3` (no `sceNpTrophy` usage) (§6).
- webMAN MOD / PS3MAPI documentation (DeepWiki mirror of `aldostools/webMAN-MOD`, `include/ps3mapi/ps3mapi_server.h`): PS3MAPI server in webMAN MOD, port 7887 native protocol + HTTP access, process/memory debugger capabilities (§7 V1–V2).
- TrueTrophies, "Persona 5 Trophies": 49 trophies, grade split, "Merciless Executioner — Perform 50 Persona fusions" (public cross-check for §2 only; SFM ids come from the console config, not from list order).
- Project-internal: `docs/RESEARCH-2026-10-22.md` (Cobra/XAI/Apollo findings, TROPUSR layouts), `docs/PROJECT-GOALS.md` (local change ≠ PSN sync), `source/probe.c` v0.0.2 (the read-only probe this document extends), README status at v0.0.10.
