This set of patches enable OVMF to support Xen.

The names of the patches are listed here:
1_ovmf_edk2_AcpiTimerLib.patch
2_ovmf_edk2_Guid_XenInfo.patch
3_ovmf_edk2_PlatformPei.patch
4_ovmf_edk2_SmbiosPlatformDxe.patch
5_ovmf_edk2_AcpiPlatformDxe.patch
2_4_5_ovmf_edk2_config_files.patch

* The first three patches make OVMF loaded by Xen hvmloader and running.
* SmbiosPlatformDxe publishes Xen hvmloader Smbios inside OVMF.
* AcpiPlatformDxe publishes Xen hvmloader Acpi tables inside OVMF. It supports both Xen and QEMU.

