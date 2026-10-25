# Hardware registration cancellation — October25,2026

**Proven:** the owner ran the first-status cancellation test on their PS3. It received one “installed” callback, canceled registration, joined the worker and completed cleanup. Installed executable identity matched the verified package. All55 known trophy files were read twice and matched the fresh complete backup; no file changes were found. Merciless Executioner remains locked.

The result follows successful disconnected emulator tests for fresh and installed authored sets, a full copied Persona5 set and changed-input rejection. Full build/review checks passed642 tests; one explicit Windows-only startup class was skipped. Native C checks ran.

This establishes cancellation for the observed console run. It does not establish completed registration, an unlock-ready context, general callback lifetime guarantees, trophy unlocking or PSN acceptance.

**Next:** a separate registration-only lifecycle that approves only the expected installed state, rejects unexpected conditions and compares the complete trophy folder before any later query or unlock step. That next test has not been built or run. The earlier [query rejection](HARDWARE-QUERY-RESULT-2026-10-25.md) and [emulator side-effect result](EMULATOR-VALIDATION-2026-10-25.md) remain valid.

Native completed-registration behavior, actual unlocking, persistence, PSN and HEN remain unverified. Public release remains the [v0.0.10 development preview](../releases/v0.0.10/README.md).
