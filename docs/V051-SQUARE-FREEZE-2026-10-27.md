# v0.0.51 Square freeze — current status

October 27, 2026. Development target: Evilnat 4.93 CFW.

**Owner-reported:** the app froze after Square with Persona 5 selected. The console restarted afterward without a reported repair prompt.

**Proven:** the retrieved v0.0.51 report ends at a per-game Review screen, with no recorded batch start or award. A later read-only console status check confirmed XMB. The historical Persona 5 saved snapshot parses as 49/49 earned and correctly rejects another batch as already complete.

**Proven, limited to authored emulator data:** an updated diagnostic exercised the actual Review/Square path on a synthetic 49/49 game. It displayed the completed-game result and exited in 8.687 seconds with zero trophy API calls and unchanged trophy files. Both updated normal app editions compile. The emulator was disconnected from Internet and PSN and used software rendering.

**Unknown:** the console freeze's cause and current live trophy-file integrity. No live trophy files were collected for this investigation. The emulator did not reproduce the hardware failure, so diagnostic changes are not a verified repair.

Square on the main game list opens All Games; Square on a trophy Review screen selects all remaining trophies in that game. The new completed-game message explains this distinction.

**Next:** preserve the installed app and reports, then run a bounded diagnostic-only hardware check. Keep Saber closed and avoid repeating the affected flow until then. Published v0.0.51 package assets and checksums have not changed. There is no new fixed package or automatic PSN sync.
