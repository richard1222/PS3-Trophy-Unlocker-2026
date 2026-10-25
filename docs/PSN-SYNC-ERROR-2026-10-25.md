# PSN sync error and local restart persistence — October 25, 2026

**Proven:** The owner clarified that error80022D1E occurred during trophy synchronization, while the trophy displays in offline mode. PSN success has not been established. Persona5 loads normally while signed out.

The owner performed a normal restart and returned to XMB signed out. Read-only collection at22:40:30 UTC double-read all57 files/3,823,830 bytes. Every file matched the preserved post-sync-error capture at22:38:44 UTC. Strict native trophy-database parsing still shows only the selected trophy earned, with the same timestamps and the other48 decoded records unchanged. This verifies local restart persistence.

**Observed changes during the earlier sync interval:** Compared with the post-unlock capture,52 files match and five changed, including the system trophy index, which grew192bytes. The selected trophy's raw sync-state field changed; that value does not prove server acceptance. Original and post-error backups remain retained. Two interrupted captures remain explicitly incomplete.

**Inference:** [Developer documentation](https://www.psdevwiki.com/ps3/Error_Codes) associates80022D1E with an installation/index mismatch scenario. That is a diagnostic lead, not a confirmed cause here. The trophy root contains three sets, so the global sync error is not yet attributable to Persona5.

**Unknown / next:** Exact failing title, sync-error cause, independent protection-file integrity, PSN acceptance and HEN compatibility. Preserve the current files while investigating. No trophy deletion, partial restore, index editing or database rebuild has been performed, and the completed award test remains disabled.

[Original native unlock evidence](FIRST-NATIVE-UNLOCK-2026-10-25.md). Public release remains v0.0.10 development preview.
