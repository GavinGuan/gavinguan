/** @file
  This code passthroughs Xen ACPI Table inside OVMF.

  Copyright (c) 2011, Bei Guan <gbtju85@gmail.com>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _XEN_ACPI_TABLE_DXE_H_
#define _XEN_ACPI_TABLE_DXE_H_


#include <PiDxe.h>

#include <Protocol/AcpiTable.h>
#include <Guid/Acpi.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Protocol/DxeSmmReadyToLock.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>

#define XEN_ACPI_PHYSICAL_ADDRESS         0x000EA020
#define XEN_BIOS_PHYSICAL_END             0x000FFFFF

extern EFI_GUID gEfiXenInfoGuid;

//
// The maximum number of tables this driver supports
//
#define EFI_ACPI_MAX_NUM_TABLES 20

UINTN mEfiAcpiMaxNumTables = EFI_ACPI_MAX_NUM_TABLES;

//
// Macro to get the high 32 bits of a 64 bit address
//
#define HIGH_PART             0x00000000FFFFFFFFULL
#define HIGH_32_BITS(a)       ( (a) & HIGH_PART )

//
// Protocol private structure definition
//
//
// ACPI support protocol instance signature definition.
//
#define EFI_ACPI_TABLE_SIGNATURE  SIGNATURE_32 ('S', 'T', 'A', 'E')

//
// Xen ACPI support protocol instance data structure
//
typedef struct {
  UINTN                                         Signature;
  EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp1;                 // Pointer to RSD_PTR structure
  EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp2;                 // Pointer to RSD_PTR structure
  EFI_ACPI_DESCRIPTION_HEADER                   *Rsdt1;                 // Pointer to RSDT table header
  EFI_ACPI_DESCRIPTION_HEADER                   *Rsdt2;                 // Pointer to RSDT table header
  EFI_ACPI_DESCRIPTION_HEADER                   *Xsdt;                  // Pointer to XSDT table header
  EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE     *Fadt1;                 // Pointer to FADT table header
  EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE     *Fadt2;                 // Pointer to FADT table header
  EFI_ACPI_1_0_FIRMWARE_ACPI_CONTROL_STRUCTURE  *Facs1;                 // Pointer to FACS table header
  EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE  *Facs2;                 // Pointer to FACS table header
  EFI_ACPI_DESCRIPTION_HEADER                   *Dsdt1;                 // Pointer to DSDT table header
  EFI_ACPI_DESCRIPTION_HEADER                   *Dsdt2;                 // Pointer to DSDT table header
  UINTN                                         NumberOfTableEntries1;  // Number of ACPI 1.0 tables
  UINTN                                         NumberOfTableEntries2;  // Number of ACPI 2.0 tables
  BOOLEAN                                       TablesInstalled1;       // ACPI 1.0 tables published
  BOOLEAN                                       TablesInstalled2;       // ACPI 2.0 tables published
  EFI_ACPI_TABLE_PROTOCOL                       AcpiTableProtocol;
} EFI_XEN_SUPPORT_ACPI_TABLE_INSTANCE;

//
// ACPI table protocol instance containing record macro
//
#define EFI_ACPI_TABLE_INSTANCE_FROM_THIS(a) \
  CR (a, \
      EFI_XEN_SUPPORT_ACPI_TABLE_INSTANCE, \
      AcpiTableProtocol, \
      EFI_ACPI_TABLE_SIGNATURE \
      )

//
// RSDT 2.0/3.0
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER                   Header;
  UINT32                                        Entry[1];
} EFI_ACPI_2_0_RSDT;

//
// XSDT 2.0/3.0
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER                   Header;
  UINT64                                        Entry[1];
} EFI_ACPI_2_0_XSDT;

//
// Common structure for Secondary System Description Tables (SSDT 2.0),
// probably including MADT, HPET, TCPA, ...
//
typedef struct {
  LIST_ENTRY                                    Link;
  UINT32                                        Signature;
  UINT32                                        Length;
  UINTN                                         XenTablePhysicalAddress;
} EFI_XEN_ACPI_SSDT_2_0_TABLE_LIST;

//
// Xen ACPI pointer
//
typedef struct {
  EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *XenRsdp1Ptr;                 // Pointer to Xen RSD_PTR structure
  EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *XenRsdp2Ptr;                 // Pointer to Xen RSD_PTR structure
  EFI_ACPI_DESCRIPTION_HEADER                   *XenRsdt1Ptr;                 // Pointer to Xen RSDT table header
  EFI_ACPI_DESCRIPTION_HEADER                   *XenRsdt2Ptr;                 // Pointer to Xen RSDT table header
  EFI_ACPI_DESCRIPTION_HEADER                   *XenXsdtPtr;                  // Pointer to Xen XSDT table header
  EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE     *XenFadt1Ptr;                 // Pointer to Xen FADT table header
  EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE     *XenFadt2Ptr;                 // Pointer to Xen FADT table header
  EFI_ACPI_1_0_FIRMWARE_ACPI_CONTROL_STRUCTURE  *XenFacs1Ptr;                 // Pointer to Xen FACS table header
  EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE  *XenFacs2Ptr;                 // Pointer to Xen FACS table header
  EFI_ACPI_DESCRIPTION_HEADER                   *XenDsdt1Ptr;                 // Pointer to Xen DSDT table header
  EFI_ACPI_DESCRIPTION_HEADER                   *XenDsdt2Ptr;                 // Pointer to Xen DSDT table header
  LIST_ENTRY                                    XenSsdtTableList;             // Link all the Xen SSDT tables
  UINTN                                         XenNumberOfTableEntries1;     // Number of Xen ACPI 1.0 tables
  UINTN                                         XenNumberOfTableEntries2;     // Number of Xen ACPI 2.0 tables
} XEN_ACPI_TABLE_POINTER;

#endif
