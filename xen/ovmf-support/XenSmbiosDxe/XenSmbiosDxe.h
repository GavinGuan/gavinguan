/** @file
  This code passthroughs Xen hvmloader SMBIOS inside OVMF.

  Copyright (c) 2011, Bei Guan <gbtju85@gmail.com>

  This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/

#ifndef _XEN_SMBIOS_DXE_H_
#define _XEN_SMBIOS_DXE_H_


#include <PiDxe.h>

#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <Guid/EventGroup.h>
#include <Guid/SmBios.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>

#define XEN_SMBIOS_PHYSICAL_ADDRESS       0x000EB000
#define XEN_SMBIOS_PHYSICAL_END           0x000F0000

extern EFI_GUID gEfiXenInfoGuid;

/**
  Figures out if Xen hvmloader provides SMBIOS for HVM.

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
SMBIOS_TABLE_ENTRY_POINT *
XenSmbiosDetect (
  VOID
  );

/**
  Get SMBIOS record by type.

  @param  Smbios       SMBIOS entry point structure pointer.
  @param  Type         Type of SMBIOS record.
  @param  Index        Index.

**/
SMBIOS_STRUCTURE_POINTER
GetSmbiosTableFromType (
  IN SMBIOS_TABLE_ENTRY_POINT  *Smbios,
  IN UINT8                 Type,
  IN UINTN                 Index
  );

/**
  Get SMBIOS String.

  @param  SmbiosTable   SMBIOS pointer.
  @param  String        SMBIOS string.

**/
CHAR8 *
GetSmbiosString (
  IN SMBIOS_STRUCTURE_POINTER  SmbiosTable,
  IN SMBIOS_TABLE_STRING       String
  );

/**
  Logs SMBIOS record.

  @param  Smbios   Pointer to SMBIOS protocol instance.
  @param  Buffer   Pointer to the data buffer.

**/
VOID
LogSmbiosData (
  IN   EFI_SMBIOS_PROTOCOL        *Smbios,
  IN   UINT8                      *Buffer
  );

/**
  Get the SMBIOS recorde from Xen SMBIOS entry point and install it.

  Type 0    BIOS Information
  Type 1    System Information
  Type 3    System Enclosure or Chassis
  Type 4    Processor Information
  Type 11   OEM Strings
  Type 16   Physical Memory Array
  Type 17   Memory Device
  Type 19   Memory Array Mapped Address
  Type 20   Memory Device Mapped Address
  Type 32   System Boot Information

  @return SMBIOS_TABLE_ENTRY_POINT   Address of Xen hvmloader SMBIOS.

**/
VOID
InstallSmbiosType0 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType1 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType3 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType4 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType11 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType16 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType17 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType19 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType20 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

VOID
InstallSmbiosType32 (
  IN SMBIOS_TABLE_ENTRY_POINT       *SmbiosEntry
  );

#endif
