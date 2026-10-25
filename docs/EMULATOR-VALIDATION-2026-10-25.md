# Emulator validation — October 25, 2026

**Proven:** disconnected PS3 emulator testing completed registration, but the operation replaced copied trophy files. The data-preservation check correctly reported failure. Rejection tests stopped before registration when inputs were changed or the activation step was absent.

This result is being held off real hardware. The real PS3 was accessed only to read and verify a fresh backup during this check; its trophy files were not modified by this work.

**Unknown:** whether Sony's native registration behaves the same way. Emulator behavior does not establish native console compatibility, trophy unlocking or PSN acceptance.

**Follow-up proven in the emulator:** the existing-set query check completed cleanup and left all55 copied trophy files unchanged. Both queries returned “context not registered”; this is a measured rejection, not query success. A changed protected file was rejected before any trophy API.

**Next:** a query-only comparison is staged on the owner's PS3 to establish native behavior, with a fresh verified backup. It has not been installed or run on hardware yet. The earlier [hardware setup milestone](WRITABLE-SETUP-HARDWARE-2026-10-25.md) remains the latest verified console result. Registration, actual unlocking and PSN acceptance remain unverified. Public release remains the [v0.0.10 development preview](../releases/v0.0.10/README.md).
