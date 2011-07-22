patch:
-----
ovmf_xen_support.patch     Enable Xen hvmloader to load OVMF
ovmf_firmware.patch        OVMF binary files
ovmf_xl_xend.patch         Add hvmloader/bios xenstore key in libxl and xend

ovmf_edk2_support_xen.patch  Enable OVMF to support Xen, publish Xen Smbios and Acpi tables inside OVMF

All the patch can be applied to the latest Xen-unstable (changeset:   23712:e298ce67777e)
and latest edk2 (r12048).
