# Registered trophy queries — October 25, 2026

**Proven:** On the owner's PS3, registration completed and both registered-context queries succeeded. The selected trophy's identifier, name, bronze grade and locked state matched expectations, with no unlock timestamp. The full bitmap reported 49 trophies, all locked. Cleanup succeeded and the console returned to the XMB. Installed-package identity was checked against the tested package.

**Preservation failure:** Two reads of each of 57 collected files agreed. Compared with the preceding native post-registration checkpoint, three files changed: the game protection file, the game trophy-user database and the system protection file. No new files or title association were observed. The combined run does not isolate which API caused each change. All decoded trophies remained locked. Original backups and the earlier four-file registration failure are retained.

**Emulator limitation:** A later single-trophy test returned API success and the expected in-memory state, but the saved native-format database failed strict validation. Inspection of the pinned emulator serializer showed it wrote only two table types while retaining the original ten-table directory. This supports an emulator serialization explanation; it is not proof of real-console persistence. The failed file has not been repaired or transferred to the PS3.

**Unknown:** Actual native unlocking, persistence after unlocking or restart, independent protection-file integrity, general callback lifetime, PSN acceptance and HEN compatibility. No explicit synchronization was requested.

**Next / current dependency:** The console is unreachable. The single-trophy update stopped during its XMB check before any app change, arming or unlock attempt. Once connectivity returns, verify the current state against the retained backup before one bounded native test and independent saved-file inspection. Public release remains v0.0.10 development preview; no working unlock release is announced.
