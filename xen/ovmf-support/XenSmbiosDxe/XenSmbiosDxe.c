/** @file
  This code passthroughs Xen hvmloader SMBIOS inside OVMF. It also installs the SMBIOS table.

  Copyright (c) 2011, Bei Guan <gbtju85@gmail.com>

  This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/

#include "XenSmbiosDxe.h"

//
// Chassis for SMBIOS entry point structure that is to be installed into EFI system config table.
//
SMBIOS_TABLE_ENTRY_POINT *EntryPointStructure    = NULL;

/**
  Figures out if Xen hvmloader provides SMBIOS for HVM.

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
SMBIOS_TABLE_ENTRY_POINT *
XenSmbiosDetect (
  VOID
  )
{

  UINTN                     XenSmbiosPtr;
  SMBIOS_TABLE_ENTRY_POINT  *XenSmbiosEntryPointStructure;
  UINTN                     Index;
  UINT8                     Length;
  UINT8                     Checksum;
  CHAR8                     *SmbiosBit;

  for (XenSmbiosPtr = XEN_SMBIOS_PHYSICAL_ADDRESS; XenSmbiosPtr < XEN_SMBIOS_PHYSICAL_END; XenSmbiosPtr += 0x10) {

    XenSmbiosEntryPointStructure = (SMBIOS_TABLE_ENTRY_POINT *) (UINTN) XenSmbiosPtr;

    if (!AsciiStrnCmp ((CHAR8 *) XenSmbiosEntryPointStructure->AnchorString, "_SM_", 4) && !AsciiStrnCmp ((CHAR8 *) XenSmbiosEntryPointStructure->IntermediateAnchorString, "_DMI_", 5)) {
      //
      // Check the structure's checksum
      //
      SmbiosBit = (CHAR8 *) XenSmbiosPtr;
      Length = XenSmbiosEntryPointStructure->EntryPointLength;
      Checksum = 0;

      for (Index = 0; Index < Length; Index++) {
        Checksum += SmbiosBit[Index];
      }
      if (Checksum != 0) {
        break;
      }

      return XenSmbiosEntryPointStructure;
    }
  }

  return NULL;
}

/**
  Copy Xen hvmloader SMBIOS contents to OVMF SMBIOS. Produce Table
  Entry Point and return the pointer to it.
  
  @param  TableEntryPointStructure   On exit, points to the SMBIOS entrypoint structure.
                                
  @retval EFI_SUCCESS                Structure created sucessfully.
  @retval EFI_NOT_READY              Some of The SMBIOS records was not available yet.
  @retval EFI_OUT_OF_RESOURCES       No enough memory.
  
**/
EFI_STATUS
EFIAPI
XenSmbiosCreateTable (
  OUT VOID    **TableEntryPointStructure
  )
{
  EFI_STATUS                      Status;
  EFI_PHYSICAL_ADDRESS            PhysicalAddress;
  SMBIOS_TABLE_ENTRY_POINT        *XenSmbiosEntryPointStructure;

  Status                        = EFI_SUCCESS;
  XenSmbiosEntryPointStructure  = NULL;

  //
  // Try to find out whether Xen hvmloader provides SMBIOS
  //
  XenSmbiosEntryPointStructure = XenSmbiosDetect ();

  if (XenSmbiosEntryPointStructure) {
    //
    // Initialize the EntryPointStructure with Xen SMBIOS
    //
    if (EntryPointStructure == NULL) {
      //
      // Allocate memory to SMBIOS Entry Point Structures (below 4GB)
      //
      PhysicalAddress = 0xffffffff;
      Status = gBS->AllocatePages (
                     AllocateMaxAddress,
                     EfiReservedMemoryType,
                     EFI_SIZE_TO_PAGES (sizeof (SMBIOS_TABLE_ENTRY_POINT)),
                     &PhysicalAddress
                    );
      if (EFI_ERROR (Status)) {
        return EFI_OUT_OF_RESOURCES;
      }

      EntryPointStructure = (SMBIOS_TABLE_ENTRY_POINT *) (UINTN) PhysicalAddress;

      CopyMem (
        EntryPointStructure,
        (VOID *) (UINTN) XenSmbiosEntryPointStructure,
        sizeof (SMBIOS_TABLE_ENTRY_POINT)
       );
    }

    //
    // Allocate memory to Structure Table (below 4GB)
    //
    PhysicalAddress = 0xffffffff;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiReservedMemoryType,
                    EFI_SIZE_TO_PAGES (EntryPointStructure->TableLength),
                    &PhysicalAddress
                    );
    if (EFI_ERROR (Status)) {
      FreePages ((VOID*) EntryPointStructure, EFI_SIZE_TO_PAGES (sizeof (SMBIOS_TABLE_ENTRY_POINT)));
      EntryPointStructure = NULL;
      return EFI_OUT_OF_RESOURCES;
    }

    CopyMem (
      (VOID *) (UINTN) PhysicalAddress,
      (VOID *) (UINTN) EntryPointStructure->TableAddress,
      EntryPointStructure->TableLength
     );

    EntryPointStructure->TableAddress = (UINT32) PhysicalAddress;

    //
    // Returns the pointer
    //
    *TableEntryPointStructure = EntryPointStructure;

  }

  return EFI_SUCCESS;
}



/**
  Installs the Xen hvmloader Smbios Table to the System Table.

  @param ImageHandle     Module's image handle
  @param SystemTable     Pointer of EFI_SYSTEM_TABLE

  @retval EFI_SUCCESS    Smbios protocol installed
  @retval Other          No protocol installed, unload driver.

**/
EFI_STATUS
EFIAPI
XenSmbiosTablePublish (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  UINT8                     *Eps;
  EFI_STATUS                Status;
  EFI_HOB_GUID_TYPE         *GuidHob;

  //
  // See if a XenInfo HOB is available
  //
  GuidHob = GetFirstGuidHob (&gEfiXenInfoGuid);
  if (GuidHob != NULL) {
    //
    // We are running inside Xen HVM, and then publish Xen hvmloader SMBIOS
    //
    Status = XenSmbiosCreateTable ((VOID **) &Eps);
    if (!EFI_ERROR (Status)) {
      gBS->InstallConfigurationTable (&gEfiSmbiosTableGuid, Eps);
    }
  }

  return EFI_SUCCESS;
}