## Latest result: read-only dependency fix verified on PS3

The owner confirmed that the corrected read-only diagnostic now displays Persona 5's trophy count, closes its harmless dialog and exits with Start. Retrieved reports confirm **49/49 earned**, two successful saved-file/parser passes, successful profile lookup after initializing its required system component, dialog closure, cleanup and XMB return.

The diagnostic's missing initialization has been corrected. The corresponding pre-session profile path in Saber is now guarded in unpublished development source. Both CFW/HEN builds and an authored two-game global-review emulator test passed, with no trophy API calls or protected-file changes.

**The installed Saber v0.0.51 package has not been replaced.** A new main-app package and bounded real-console validation are still needed. The exact cause of the original per-game v0.0.51 freeze remains unproven. This investigation awarded no trophies, performed no PSN sync and establishes no new HEN hardware compatibility.

## Read-only hardware follow-up

The separate read-only diagnostic also stalled after Square on the owner's console. Its live report was retrieved successfully and stops before the first saved-file read marker. This narrows that diagnostic's failure to initial screen presentation, system callbacks, active-profile lookup or reporting; it does not establish which operation failed or explain the original v0.0.51 issue.

A second read-only diagnostic separates those phases. Its authored completed-game test and final-package startup checks passed in disconnected emulation, with no trophy API calls or saved-file changes. It is staged for an owner-controlled console test; that result is pending. The original freeze remains unresolved, and no fixed unlocker release or new PSN result is claimed.

# v0.0.51 Square freeze — current status

October 27, 2026. Development target: Evilnat 4.93 CFW.

**Owner-reported:** the app froze after Square with Persona 5 selected. The console restarted afterward without a reported repair prompt.

**Proven:** the retrieved v0.0.51 report ends at a per-game Review screen, with no recorded batch start or award. A later read-only console status check confirmed XMB. The historical Persona 5 saved snapshot parses as 49/49 earned and correctly rejects another batch as already complete.

**Proven, limited to authored emulator data:** an updated diagnostic exercised the actual Review/Square path on a synthetic 49/49 game. It displayed the completed-game result and exited in 8.687 seconds with zero trophy API calls and unchanged trophy files. Both updated normal app editions compile. The emulator was disconnected from Internet and PSN and used software rendering.

**Unknown:** the console freeze's cause and current live trophy-file integrity. No live trophy files were collected for this investigation. The emulator did not reproduce the hardware failure, so diagnostic changes are not a verified repair.

Square on the main game list opens All Games; Square on a trophy Review screen selects all remaining trophies in that game. The new completed-game message explains this distinction.

**Next:** preserve the installed app and reports, then run a bounded diagnostic-only hardware check. Keep Saber closed and avoid repeating the affected flow until then. Published v0.0.51 package assets and checksums have not changed. There is no new fixed package or automatic PSN sync.
