# PS3 Trophy Unlocker 2026 — Muse research addendum: the October 2026 PS3 key discoveries and what they change for this project

- Date: October 23, 2026. Author: Muse (mobile), companion to `PS3-Trophy-Unlocker-Muse-Research.md` (same day).
- Question asked by the owner: investigate the October 2026 key discoveries alongside the trophy research, and identify how the findings can improve the project's code **without dumping the console's private keys**.
- Short answer: the discoveries are real, public, and useful to this project — but at the firmware-analysis layer, not the trophy layer. They complete the public key set for decrypting PS3 firmware components on a PC, which lets us study the firmware's own code (including trophy-adjacent system modules) across variants from public files. They do **not** change the sceNpTrophy lifecycle, trophy file formats, or PSN behavior, and nothing in them is, or needs to be, a private key from the owner's console.
- Labeling: **[VERIFIED]** = read from a primary/canonical source for this document. **[REPORTED]** = consistent secondary reporting of a primary post not directly readable here (X posts). **[ANALYSIS]** = my reasoning about consequences.

## 1. What was actually discovered, in sequence

**Background (long-standing, [VERIFIED] from PSDevWiki's Keys page).** PS3s fall into two boot-chain generations. Pre-LV0.2 consoles (Fat and Slims up to CECH-25xx with minver ≤ 3.56) use the original `metldr`/`lv0ldr`, whose decryption keys have been public since 2010–2012. LV0.2 consoles (CECH-30xx+ Slims and all Super Slims) use `metldr.2`/`lv0ldr.2`, checked with a second signature layer; their loader keys were **not** public, which is why those models were limited to PS3HEN and, later, BadWDSD-hardware QuasiCFW.

**Step 1 — the BootROM work (2025 → September 2026) [REPORTED].** A team credited in zecoxao's write-up *"Glitching the 'Almost' Perfect Code"* (wildcard, ZeroTolerance, jestero, MikeM64, Proxima, Anonymous, kafuu, esc0rtd3w, bguerville, and others) decapped the Cell's SPU regions (a first decap returned Christmas 2025 with the wrong ROM — the PPE microcode ROM), recovered the isolated loader ROM (0x400 bytes decoded) that implements the AES unwrap path for lv0ldr/metldr, and established that the root key material arrives as 32-bit reads on undocumented SPU channel 66 and is then locked away via channel 64 — leaving glitching (or a DPA-class attack) as the extraction path.

**Step 2 — first per-console lv0ldr body key (~September 26, 2026) [REPORTED].** Zecoxao published the write-up after a CECHL Fat yielded a real **per-console** lv0ldr body key via SPU glitching: a Python decrypt of that console's `bootldr.bin` with the MAC check passing and 191,152 bytes of plaintext. Note the scope precisely: this key is per-console (it decrypts that class of console's bootldr layer) and it was the *researchers'* console — it is a method proof, not a global key, and it is not needed by this project (§3, rule 3).

**Step 3 — the .2 loaders decrypted (October 22, 2026) [REPORTED, then VERIFIED in effect].** Zecoxao posted "it's over" with the first decrypted **metldr.2** and **lv0ldr.2** samples — the two loaders that gate the LV0.2 boot chain. Scene coverage within a day (onejailbreak's "PS3 metldr.2 and lv0ldr.2 Open Super Slim CFW Path", Oct 22, 2026; Modded Warfare's jailbreak-news roundup) describes it as the step that opens a path to custom loaders on late Slims and Super Slims without relying on a hardware inject.

**Step 4 — keys in the canonical public tables [VERIFIED].** PSDevWiki's Keys page (read for this document, current revision) now carries dedicated sections for both loaders with complete rows for firmware **3.60 ~ 4.93**:

| Loader | ERK (AES key, 32 B) | RIV (16 B) | ECDSA PUBLIC | ECDSA PRIVATE | Curve |
| --- | --- | --- | --- | --- | --- |
| Lv0Ldr.2 | published | published | published | **N.A.** | 0x20 |
| MetLdr.2 | published | published | published | **N.A.** | 0x10 |

Two facts in that table bound every claim made about this discovery:

- Anyone can now **decrypt and verify** metldr.2/lv0ldr.2 from any LV0.2 console's firmware/flash image on a PC. The boot chain of every PS3 model is now readable end to end from public material.
- The .2 **signing private keys are not published** ("N.A.") and cannot be derived the 2012 way (that attack needed Sony's broken ECDSA nonce generation, which is not present here). So "CFW for Super Slim" is an opened path — the loader code is now analyzable for a signature-bypass or delivery route (e.g., BadWDSD-class injection) — not a shipped result as of this writing. Any source claiming finished Super Slim CFW from this alone is ahead of the evidence.

## 2. What this changes for the Trophy Unlocker code [ANALYSIS, grounded in §1]

1. **Firmware-module research moves entirely off-console.** The project's main open unknown (companion doc §10) is whether trophy-relevant system behavior differs across CEX/DEX/D-PEX variants. With every loader generation decryptable, the firmware's system modules can be extracted from public update files on a PC and compared variant-to-variant — no console dumping, no per-console keys, and no risk to the owner's machine. Concrete next use: diff the trophy-adjacent firmware modules between the CEX and D-PEX 4.93 builds and record the result in `knowledge/`.
2. **Keep the code variant- and model-agnostic.** If a custom-loader path lands for LV0.2 models, the project's potential base widens beyond pre-LV0.2 CFW consoles. Code that detects capabilities (API availability, file locations, firmware variant strings) will port; code that assumes one boot-chain generation or one CFW's internals will not. Practical step: the probe's report header should record firmware variant **and** boot-chain generation (pre-LV0.2 vs LV0.2), so future console evidence is never silently pooled across generations.
3. **Key hygiene rule — now fully satisfiable, make it explicit.** Every key this project's tooling could legitimately need for decryption or verification is public: the loader keys (now complete), the long-public application/NPDRM public material, and the trophy/save-data handling keys already used by public tools such as Apollo. The codebase should reference public key material from canonical public sources (PSDevWiki's Keys page is the natural citation) and must **never** request, dump, or store per-console keys (EID root key and friends). The October work makes this rule *easier* to keep, not harder: there is no longer any firmware component the project might be tempted to extract a console key to reach.
4. **No change at the trophy layer — do not let the news rewrite the plan.** The discoveries sit below the operating system. They do not alter: the sceNpTrophy lifecycle or its per-process context model; TROPUSR/TROPCONF formats or their (already public) handling; the read-only validation strategy (V1–V4) or the staged gates G0–G4; or anything about PSN synchronization. The owner's own console (CECH-2001A, pre-LV0.2) never needed the .2 keys at all. Treat this addendum as an expansion of the project's *research* surface and future compatibility, not as a new unlock mechanism.

## 3. Recommended code-level actions

- Add a `firmware/` research note series to the repo: for each firmware variant the project targets, record (from decrypted **public** update files) the identity/hash of the trophy-adjacent system modules, so variant drift is measured instead of assumed. (Research only; no console files.)
- Extend the probe report header with boot-chain generation, per §2.2.
- Add a line to the project's research index stating the key policy of §2.3: public keys only, cited to PSDevWiki; per-console keys out of scope by design.
- Revisit compatibility claims only when a verifiable LV0.2 CFW actually ships; until then, README language ("Initial target: Evilnat 4.93 CFW") stays accurate as-is.

## 4. Sources

- PSDevWiki, "Keys" (current revision, read 2026-10-23): Lv0Ldr.2 and MetLdr.2 tables (3.60~4.93; ERK/RIV/PUBLIC published, PRIVATE N.A.; curves 0x20/0x10); historical lv0ldr/metldr tables for contrast (§1).
- onejailbreak, "PS3 metldr.2 and lv0ldr.2 Open Super Slim CFW Path" (2026-10-22) and "PS3 BootROM Research Recovers First lv0ldr Body Key" (~2026-09-26): the Step 2/3 reporting, team credits, SPU channel 66/64 mechanism, CECHL decrypt details (§1). [REPORTED tier — secondary write-ups of zecoxao's posts/write-up.]
- Modded Warfare, "Jailbreak News" (2026-10-22/23): the "PS3 keys dumped for super-slim and later slims" framing and the link to zecoxao's X post (§1).
- PSDevWiki, "Vulnerabilities" (BadWDSD / XDR-DRAM sections): the existing hardware-inject QuasiCFW path for LV0.2 models that the .2 decryption is reported to supersede (§1).
- Historical context: 2012 LV0 key release reporting (The Escapist/Eurogamer) for why LV0.2's second signature layer existed (§1).
