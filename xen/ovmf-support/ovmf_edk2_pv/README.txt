1) OvmfPkg/ParaVirtualizationDxe installs the Xen Pv protocol EFI_XEN_HV_PROTOCOL. Other Dxe can consume the protocol in itself.
2) Protocol EFI_XEN_HV_PROTOCOL is implemented in OvmfPkg/Include/Protocol/Hypercall.h. It takes all the Xen hypercalls (38+8) as protocol methods.
3) In OvmfPkg/ParaVirtualizationDxe/Hypercall.c, all hypercalls call one EFI ABI fucntion, which has beeb implemented in X64 and IA32. See OvmfPkg/ParaVirtualizationDxe/[Ia32|X64]/GasketHypercall.S.


