/** @file
  Publish Xen hvmloader SMBIOS inside OVMF. This code parses Xen SMBIOS tables and add them into UEFI SMBIOS by calling EFI_SMBIOS_PROTOCOL=>Add.

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
// SMBIOS entry point structure
//
SMBIOS_TABLE_ENTRY_POINT    *EntryPointStructure    = NULL;
EFI_SMBIOS_PROTOCOL         *gSmbios;

/**
  Type 0 -- BIOS Information

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType0 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // BIOS info (TYPE 0)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 0, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 0 (BIOS Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type0
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type0);

  return;
}

/**
  Type 1 -- System Information

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType1 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // System info (TYPE 1)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 1, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 1 (System Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type1
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type1);

  return;
}

/**
  Type 3 -- System Enclosure or Chassis

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType3 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // Enclosure or Chassis info (TYPE 3)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 3, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 3 (Enclosure or Chassis Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type3
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type3);

  return;
}

/**
  Type 4 -- Processor Information

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType4 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // Processor info (TYPE 4)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 4, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 4 (Processor Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type4
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type4);

  return ;
}

/**
  Type 11 -- OEM Strings

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType11 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // OEM info (TYPE 11)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 11, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 11 (OEM Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type11
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type11);

  return ;
}

/**
  Type 16 -- Physical Memory Array

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType16 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // Physical Memory Array info (TYPE 16)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 16, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 16 (Physical Memory Array info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type16
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type16);

  return ;
}

/**
  Type 17 -- Memory Device

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType17 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // Memory Device info (TYPE 17)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 17, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 17 (Memory Device Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type17
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type17);

  return ;
}

/**
  Type 19 -- Memory Array Mapped Address

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType19 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // Memory Array Mapped Address info (TYPE 19)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 19, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 19 (Memory Array Mapped Address Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type19
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type19);

  return ;
}

/**
  Type 20 -- Memory Device Mapped Address

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType20 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // Memory Device Mapped Address info (TYPE 20)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 20, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 20 (Memory Device Mapped Address Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type20
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type20);

  return ;
}

/**
  Type 32 -- System Boot Information

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType32 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  )
{
  SMBIOS_STRUCTURE_POINTER          SmbiosTable;

  //
  // System Boot info (TYPE 32)
  // 
  SmbiosTable = GetSmbiosTableFromType (SmbiosEntry, 32, 0);
  if (SmbiosTable.Raw == NULL) {
    DEBUG ((EFI_D_ERROR, "SmbiosTable: Type 32 (System Boot Info) not found!\n"));
    return ;
  }

  //
  // Log Smbios Record Type32
  //
  LogSmbiosData(gSmbios,(UINT8*)SmbiosTable.Type32);

  return ;
}

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
  Get SMBIOS record length.

  @param  SmbiosTable   SMBIOS pointer.

**/
UINTN
SmbiosTableLength (
  IN SMBIOS_STRUCTURE_POINTER SmbiosTable
  )
{
  CHAR8  *AChar;
  UINTN  Length;

  AChar = (CHAR8 *)(SmbiosTable.Raw + SmbiosTable.Hdr->Length);

  //
  // Each structure shall be terminated by a double-null (SMBIOS spec.7.1)
  //
  while ((*AChar != 0) || (*(AChar + 1) != 0)) {
    AChar ++;
  }
  Length = ((UINTN)AChar - (UINTN)SmbiosTable.Raw + 2);
  
  return Length;
}

/**
  Get SMBIOS record by type.

  @param  Smbios       SMBIOS entry point structure pointer.
  @param  Type         Type of SMBIOS record.
  @param  Index        Index.

**/
SMBIOS_STRUCTURE_POINTER
GetSmbiosTableFromType (
  IN SMBIOS_TABLE_ENTRY_POINT  *Smbios,
  IN UINT8                     Type,
  IN UINTN                     Index
  )
{
  SMBIOS_STRUCTURE_POINTER SmbiosTable;
  UINTN                    SmbiosTypeIndex;
  
  SmbiosTypeIndex = 0;
  SmbiosTable.Raw = (UINT8 *)(UINTN)Smbios->TableAddress;
  if (SmbiosTable.Raw == NULL) {
    return SmbiosTable;
  }
  while ((SmbiosTypeIndex != Index) || (SmbiosTable.Hdr->Type != Type)) {
    if (SmbiosTable.Hdr->Type == 127) {
      SmbiosTable.Raw = NULL;
      return SmbiosTable;
    }
    if (SmbiosTable.Hdr->Type == Type) {
      SmbiosTypeIndex ++;
    }

    //
    // Get the structure address
    //
    SmbiosTable.Raw = (UINT8 *)(SmbiosTable.Raw + SmbiosTableLength (SmbiosTable));
  }

  return SmbiosTable;
}

/**
  Get SMBIOS String.

  @param  SmbiosTable   SMBIOS pointer.
  @param  String        SMBIOS string.

**/
CHAR8 *
GetSmbiosString (
  IN SMBIOS_STRUCTURE_POINTER  SmbiosTable,
  IN SMBIOS_TABLE_STRING       String
  )
{
  CHAR8      *AString;
  UINT8      Index;

  Index = 1;
  AString = (CHAR8 *)(SmbiosTable.Raw + SmbiosTable.Hdr->Length);
  while (Index != String) {
    while (*AString != 0) {
      AString ++;
    }
    AString ++;
    if (*AString == 0) {
      return AString;
    }
    Index ++;
  }

  return AString;
}

/**
  Log SMBIOS record.

  @param  Smbios   Pointer to SMBIOS protocol instance.
  @param  Buffer   Pointer to the data buffer.

**/
VOID
LogSmbiosData (
  IN   EFI_SMBIOS_PROTOCOL        *Smbios,
  IN   UINT8                      *Buffer
  )
{
  EFI_STATUS         Status;
  EFI_SMBIOS_HANDLE  SmbiosHandle;
  
  SmbiosHandle = 0;
  Status = Smbios->Add (
                     Smbios,
                     NULL,
                     &SmbiosHandle,
                     (EFI_SMBIOS_TABLE_HEADER*)Buffer
                     );
  ASSERT_EFI_ERROR (Status);
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
SmbiosTablePublishEntry (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_HOB_GUID_TYPE         *GuidHob;

  //
  // See if a XenInfo HOB is available
  //
  GuidHob = GetFirstGuidHob (&gEfiXenInfoGuid);
  if (GuidHob != NULL) {
    //
    // We are running inside Xen HVM. Then, find the Xen SMBIOS.
    //
    EntryPointStructure = XenSmbiosDetect();
    if (EntryPointStructure == NULL) {
      return EFI_NOT_FOUND;
    }

    //
    // Find the SMBIOS protocol 
    //
    Status = gBS->LocateProtocol (
                    &gEfiSmbiosProtocolGuid,
                    NULL,
                    (VOID**)&gSmbios
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    InstallSmbiosType0 (EntryPointStructure);
    InstallSmbiosType1 (EntryPointStructure);
    InstallSmbiosType3 (EntryPointStructure);
    InstallSmbiosType4 (EntryPointStructure);
    InstallSmbiosType11 (EntryPointStructure);
    InstallSmbiosType16 (EntryPointStructure);
    InstallSmbiosType17 (EntryPointStructure);
    InstallSmbiosType19 (EntryPointStructure);
    InstallSmbiosType20 (EntryPointStructure);
    InstallSmbiosType32 (EntryPointStructure);
  }

  return EFI_SUCCESS;
}
