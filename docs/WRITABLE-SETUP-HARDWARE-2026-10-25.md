# Writable context setup passed on PS3 — October 25, 2026

**Proven:** the owner-run v0.0.27 setup-only diagnostic successfully created and cleaned up an app-owned writable-capable trophy context and handle on the tested PS3. Its report passed verification with no retained runtime, context or handle.

The installed executable matched the verified package payload. Independent double reads of all four trophy-data files matched the preserved baseline; no protected-data change was observed.

**Inference:** this context-creation path can support further native registration research. Context creation alone does not establish that registration or unlocking will succeed.

**Unknown:** retail registration, callback lifetime, actual unlocking, persistence after unlocking, PSN synchronization, HEN compatibility and broader hardware support. Signature authentication and PSN acceptance are not established by context creation.

This test performed no trophy-state query: its zero count is unqueried output. The earlier v0.0.25 read-only query established49 trophies and a locked selected trophy; the corresponding record remains unchanged.

The next step is a separate guarded registration-only diagnostic with title-pack checks, callback/worker handling and before/after backups. User/file guards remain bounded observations rather than atomic protection against every external change. Firmware target remains owner-reported Evilnat4.93 CFW; no fresh firmware or PSN-state attestation was performed during collection.

No installable package or working unlocker is released publicly with this update. The public release remains the [v0.0.10 development preview](../releases/v0.0.10/README.md). Implementation and raw console data remain private.
