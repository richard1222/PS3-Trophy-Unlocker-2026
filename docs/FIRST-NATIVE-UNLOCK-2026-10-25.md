# First native single-trophy unlock — October 25, 2026

**Proven:** Persona 5's bronze trophy “Merciless Executioner” (ID11) unlocked on the owner's PS3 through one native award call. The API returned success; subsequent queries showed the selected trophy earned with a timestamp and no additional earned trophies. Cleanup succeeded and the console returned to XMB.

**Independent saved-file proof:** Two complete collections, at22:26:43 and22:27:48 UTC, agreed on all57 files and3,823,638 bytes. Strict parsing of the native saved trophy database found only ID11 unlocked with nonzero timestamps. The other48 decoded trophy records were unchanged from the retained pre-unlock checkpoint. Four files changed, including the trophy-user and transaction databases and two protection files. No files were added or removed. This proves immediate local disk persistence; byte preservation was not expected or claimed for the requested award.

**Failures retained:** An initial check occurred while the app was still running; a separate FTP capture timed out and remains incomplete. The two completed captures above independently agreed. Earlier registration/query preservation failures and the emulator's invalid native-format save remain documented; none were erased or converted into passes.

**Inference:** Native hardware persistence differs from the observed emulator serializer failure. This single measured console result does not establish broad compatibility.

**Unknown:** Local Trophy Collection presentation, persistence after a normal restart, independent protection-file integrity, general callback lifetime, PSN acceptance and HEN compatibility. A local sync-state value does not prove PSN synchronization. No explicit sync was requested.

**Next:** Confirm the selected trophy in the local XMB Trophy Collection, then check it after a normal restart. The completed test has been disabled to prevent repetition. Original backups and evidence are retained. Public release remains v0.0.10 development preview; this result announces no new installable release.
