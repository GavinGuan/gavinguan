patch:
-----
ovmf_edk2_OvmfPkg_AcpiPlatformDxe.patch
ovmf_edk2_OvmfPkg_SmbiosPlatformDxe.patch
ovmf_edk2_OvmfPkg_configuration.patch
ovmf_xen_smbios_acpi_publish.patch

(PS: All the ovmf_edk2_* patches are included in one: ovmf_edk2_OvmfPkg_final.patch)

binary file
-----
ovmf_binary_for_xen\ovmf-ia32.bin
ovmf_binary_for_xen\ovmf-ia32-cirrus-vga.bin
ovmf_binary_for_xen\ovmf-x64.bin
ovmf_binary_for_xen\ovmf-x64-cirrus-vga.bin

Xen ACPI Tables published inside UEFI
-----
1) The patches publish Xen ACPI tables inside UEFI. Support both QEMU and Xen.
2) Create a new OvmfPkg/AcpiPlatformDxe to replace MdeModulePkg/Universal/Acpi/AcpiPlatformDxe.
3) OvmfPkg/AcpiPlatformDxe detects the Xen environment, finds out Xen ACPI Tables and publishes them with the Acpi Table protocol. If it is not the Xen environment, OvmfPkg/AcpiPlatformDxe just does as MdeModulePkg/Universal/Acpi/AcpiPlatformDxe.

Xen SMBIOS published inside UEFI
-----
1) The patches publish Xen SMBIOS inside UEFI. Support both QEMU and Xen.
2) OvmfPkg/SmbiosPlatformDxe can detect the Xen environment and Xen hvmloader SMBIOS. After that, it parses Xen BIOS structure and calls EFI_SMBIOS_PROTOCOL=>Add to add them to UEFI system.