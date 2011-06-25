Xen ACPI Tables published inside UEFI
-----
1) The patches publish Xen ACPI tables inside UEFI. They support both QEMU and Xen.
2) OvmfPkg/XenAcpiTableDxe can detect the Xen environment and XenACPI Tables, and then installs Xen ACPI Tables to UEFI system configuration table. 
3) MdeModulePkg/Universal/Acpi/AcpiTableDxe will look up the system table to see if ACPI Tables have been installed. If they have been installed, just does nothing. Otherwise, it does as before.

patch:
ovmf_edk2_OvmfPkg_XenAcpiTableDxe.patch
ovmf_edk2_AcpiTableDxe.patch
ovmf_xen_smbios_acpi_publish.patch
ovmf_binary_for_xen\ovmf-ia32.bin
ovmf_binary_for_xen\ovmf-ia32-cirrus-vga.bin
ovmf_binary_for_xen\ovmf-x64.bin
ovmf_binary_for_xen\ovmf-x64-cirrus-vga.bin


Xen SMBIOS published inside UEFI
-----
1) The patches publish Xen SMBIOS inside UEFI. They support both QEMU and Xen.
2) OvmfPkg/XenSmbiosDxe can detect the Xen environment and Xen hvmloader SMBIOS, and then installs Xen BIOS to UEFI system configuration table. 
3) Then, MdeModulePkg/Universal/SmbiosDxe will look up the system table to see if a SMBIOS table has been install. If it has been installed, just does nothing.(Support Xen environment) Otherwise, it does as before.(Support QEMU environment)

patch:
ovmf_edk2_OvmfPkg_XenSmbiosDxe.patch
ovmf_edk2_SmbiosDxe.patch
ovmf_xen_smbios_acpi_publish.patch
