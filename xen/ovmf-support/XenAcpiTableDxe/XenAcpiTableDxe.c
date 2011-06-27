/** @file
  This code passthroughs and installs Xen hvmloader ACPI Table inside OVMF.

  Copyright (c) 2011, Bei Guan <gbtju85@gmail.com>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

//
// Includes
//
#include "XenAcpiTableDxe.h"

//
// Handle to install ACPI Table Protocol
//
XEN_ACPI_TABLE_POINTER                             *XenAcpiTablePointerStructure;
EFI_XEN_SUPPORT_ACPI_TABLE_INSTANCE                *AcpiTableInstance;

/**
  This function calculates and updates an UINT8 checksum.

  @param  Buffer          Pointer to buffer to checksum
  @param  Size            Number of bytes to checksum

  @return EFI_SUCCESS             The function completed successfully.

**/
EFI_STATUS
VerifyAcpiPlatformChecksum (
  IN VOID       *Buffer,
  IN UINTN      Size
  )
{
  UINT8 Sum;
  UINT8 *Ptr;

  Sum = 0;
  //
  // Initialize pointer
  //
  Ptr = Buffer;

  //
  // Add all content of buffer
  //
  while ((Size--) != 0) {
    Sum = (UINT8) (Sum + (*Ptr++));
  }
  //
  // Check checksum
  //
  if (Sum != 0){
    return EFI_CRC_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  This function calculates and updates an UINT8 checksum.

  @param  Buffer          Pointer to buffer to checksum
  @param  Size            Number of bytes to checksum
  @param  ChecksumOffset  Offset to place the checksum result in

  @return EFI_SUCCESS     The function completed successfully.

**/
EFI_STATUS
AcpiPlatformChecksum (
  IN VOID       *Buffer,
  IN UINTN      Size,
  IN UINTN      ChecksumOffset
  )
{
  UINT8 Sum;
  UINT8 *Ptr;

  Sum = 0;
  //
  // Initialize pointer
  //
  Ptr = Buffer;

  //
  // set checksum to 0 first
  //
  Ptr[ChecksumOffset] = 0;

  //
  // add all content of buffer
  //
  while ((Size--) != 0) {
    Sum = (UINT8) (Sum + (*Ptr++));
  }
  //
  // set checksum
  //
  Ptr                 = Buffer;
  Ptr[ChecksumOffset] = (UINT8) (0xff - Sum + 1);

  return EFI_SUCCESS;
}

/**
  Checksum all versions of the common tables, RSDP, RSDT, XSDT.

  @param  AcpiTableInstance  Protocol instance private data.

  @return EFI_SUCCESS        The function completed successfully.

**/
EFI_STATUS
ChecksumCommonTables (
  IN OUT EFI_XEN_SUPPORT_ACPI_TABLE_INSTANCE         *AcpiTableInstance
  )
{
  //
  // RSDP ACPI 1.0 checksum for 1.0 table.  This is only the first 20 bytes of the structure
  //
  AcpiPlatformChecksum (
    AcpiTableInstance->Rsdp1,
    sizeof (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER),
    OFFSET_OF (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER,
    Checksum)
    );

  //
  // RSDP ACPI 1.0 checksum for 2.0/3.0 table.  This is only the first 20 bytes of the structure
  //
  AcpiPlatformChecksum (
    AcpiTableInstance->Rsdp2,
    sizeof (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER),
    OFFSET_OF (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER,
    Checksum)
    );

  //
  // RSDP ACPI 2.0/3.0 checksum, this is the entire table
  //
  AcpiPlatformChecksum (
    AcpiTableInstance->Rsdp2,
    sizeof (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER),
    OFFSET_OF (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER,
    ExtendedChecksum)
    );

  //
  // RSDT checksums
  //
  AcpiPlatformChecksum (
    AcpiTableInstance->Rsdt1,
    AcpiTableInstance->Rsdt1->Length,
    OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER,
    Checksum)
    );

  AcpiPlatformChecksum (
    AcpiTableInstance->Rsdt2,
    AcpiTableInstance->Rsdt2->Length,
    OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER,
    Checksum)
    );

  //
  // XSDT checksum
  //
  AcpiPlatformChecksum (
    AcpiTableInstance->Xsdt,
    AcpiTableInstance->Xsdt->Length,
    OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER,
    Checksum)
    );
  
  return EFI_SUCCESS;
}

/**
  Initialize the XenAcpiTablePointer Structure

  @return EFI_SUCCESS             The function completed successfully.

**/
EFI_STATUS
EFIAPI
InitializeXenAcpiTablePointer (
  IN OUT XEN_ACPI_TABLE_POINTER                       *XenAcpiTablePointerStructure;
  )
{
  XenAcpiTablePointerStructure->XenRsdp1Ptr = NULL;
  XenAcpiTablePointerStructure->XenRsdp2Ptr = NULL;  // Xen RSDP 2.0
  XenAcpiTablePointerStructure->XenRsdt1Ptr = NULL;
  XenAcpiTablePointerStructure->XenRsdt2Ptr = NULL;  // Xen RSDT 2.0
  XenAcpiTablePointerStructure->XenXsdtPtr  = NULL;  // Xen XSDT 2.0
  XenAcpiTablePointerStructure->XenFadt1Ptr = NULL;  // Xen FADT 1.0
  XenAcpiTablePointerStructure->XenFadt2Ptr = NULL;  // Xen FADT 2.0
  XenAcpiTablePointerStructure->XenFacs1Ptr = NULL;
  XenAcpiTablePointerStructure->XenFacs2Ptr = NULL;  // Xen FACS 2.0
  XenAcpiTablePointerStructure->XenDsdt1Ptr = NULL;
  XenAcpiTablePointerStructure->XenDsdt2Ptr = NULL;  // Xen DSDT

  XenAcpiTablePointerStructure->XenNumberOfTableEntries1 = 0;
  XenAcpiTablePointerStructure->XenNumberOfTableEntries2 = 0;

  return EFI_SUCCESS;
}

/**
  Figures out if Xen hvmloader provides ACPI Table for HVM.
  And then, get the pointers of each table

  @return EFI_SUCCESS             The function completed successfully.

**/
EFI_STATUS
EFIAPI
XenAcpiTableDetect (
  IN OUT XEN_ACPI_TABLE_POINTER                       *XenAcpiTablePointerStructure
  )
{
  UINTN                                        XenAcpiPtr;

  EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *Rsdp2Structure;
  EFI_ACPI_2_0_RSDT                            *Rsdt2Structure; //Ptr to RSDT2 header
  EFI_ACPI_2_0_XSDT                            *XsdtStructure;  //Ptr to XSDT2 header
  EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE    *Fadt2Structure; //Ptr to FADT header
  EFI_ACPI_DESCRIPTION_HEADER                  *AcpiTableHeader; //Ptr to DSDT header

  EFI_ACPI_TABLE_VERSION                       Version;
  EFI_STATUS                                   Status;
  EFI_XEN_ACPI_SSDT_2_0_TABLE_LIST             *CurrentTableList;
  UINT64                                       Buffer64;
  UINTN                                        Index;
  UINT8                                        Length;

  //
  // Initialize the XEN_ACPI_TABLE_POINTER
  //
  InitializeXenAcpiTablePointer(XenAcpiTablePointerStructure);
  InitializeListHead (&XenAcpiTablePointerStructure->XenSsdtTableList);

  //
  // Detect the RSDP table
  //
  for (XenAcpiPtr = XEN_ACPI_PHYSICAL_ADDRESS; XenAcpiPtr < XEN_BIOS_PHYSICAL_END; XenAcpiPtr += 0x10) {

    Rsdp2Structure = (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *) (UINTN) XenAcpiPtr;

    if (!AsciiStrnCmp ((CHAR8 *) &Rsdp2Structure->Signature, "RSD PTR ", 8)) {
      //
      // Detect the Acpi Version
      //
      Version = Rsdp2Structure->Revision;

      //
      // RSDP ACPI 1.0 checksum for 2.0/3.0 table.  This is only the first 20 bytes of the structure
      //
      Status = VerifyAcpiPlatformChecksum (
        Rsdp2Structure,
        sizeof (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER)
        );

      //
      // For RSDP 2.0 (Xen-4.2 only has a RSDP 2.0 table)
      //
      if (Version >= 2) {

        //
        // RSDP ACPI 2.0/3.0 checksum, this is the entire table
        //
        Status = VerifyAcpiPlatformChecksum (
          Rsdp2Structure,
          sizeof (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER)
          );

        if (Status != EFI_SUCCESS) {
          break;
        }

        //
        // Get Xen RSDP pointer, and then RSDT2.0 and XSDT pointers
        //
        XenAcpiTablePointerStructure->XenRsdp2Ptr = Rsdp2Structure;
        XenAcpiTablePointerStructure->XenRsdt2Ptr = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp2Structure->RsdtAddress;
        XenAcpiTablePointerStructure->XenXsdtPtr  = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp2Structure->XsdtAddress;

        //
        // Detect the RSDT table, get the FADT 1.0 pointer
        //
        Rsdt2Structure = (EFI_ACPI_2_0_RSDT *) (UINTN) XenAcpiTablePointerStructure->XenRsdt2Ptr;

        if (!AsciiStrnCmp ((CHAR8 *) &Rsdt2Structure->Header.Signature, "RSDT", 4)) {

          Status = VerifyAcpiPlatformChecksum (
            Rsdt2Structure,
            Rsdt2Structure->Header.Length
            );

          if (Status != EFI_SUCCESS) {
            break;
          }

          //
          // The first one is FADT 1.0 table
          //
          XenAcpiTablePointerStructure->XenFadt1Ptr = (EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE *) (UINTN) Rsdt2Structure->Entry[0];

          //
          // Calculate the Number tables pointed by RSDT/XSDT (FADT1.0/2.0 and SSDTs)
          //
          XenAcpiTablePointerStructure->XenNumberOfTableEntries2 = (XenAcpiTablePointerStructure->XenRsdt2Ptr->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER)) / sizeof (UINT32);
        }

        //
        // Detect the XSDT 2.0 table, get the FADT 2.0 pointer
        //
        XsdtStructure = (EFI_ACPI_2_0_XSDT *) (UINTN) XenAcpiTablePointerStructure->XenXsdtPtr;

        if (!AsciiStrnCmp ((CHAR8 *) &XsdtStructure->Header.Signature, "XSDT", 4)) {

          Status = VerifyAcpiPlatformChecksum (
            XsdtStructure,
            XsdtStructure->Header.Length
            );

          if (Status != EFI_SUCCESS) {
            break;
          }

          //
          // The first one is FADT 2.0 table
          //
          CopyMem (
            &Buffer64,
            (VOID *) (UINTN) XenAcpiTablePointerStructure->XenXsdtPtr + sizeof(EFI_ACPI_DESCRIPTION_HEADER),
            sizeof (UINT64)
           );

           XenAcpiTablePointerStructure->XenFadt2Ptr = (EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *) (UINTN) HIGH_32_BITS(Buffer64);
           Fadt2Structure = XenAcpiTablePointerStructure->XenFadt2Ptr;


          //
          // Following tables are the Secondary System Description Tables (SSDT), we use a list to store them
          // RSDT 2.0 and XSDT 2.0 share the same SSDT in Xen-4.2
          //
          Length = XenAcpiTablePointerStructure->XenNumberOfTableEntries2;
          for (Index = 1; Index < Length; Index++) {
            //
            // Create a new list entry
            //
            CurrentTableList = AllocatePool (sizeof (EFI_XEN_ACPI_SSDT_2_0_TABLE_LIST));
            ASSERT (CurrentTableList);

            CopyMem (
              &Buffer64,
              (VOID *) (UINTN) XenAcpiTablePointerStructure->XenXsdtPtr + sizeof(EFI_ACPI_DESCRIPTION_HEADER) + Index * sizeof (UINT64),
              sizeof (UINT64)
             );
            CurrentTableList->XenTablePhysicalAddress = (UINTN) HIGH_32_BITS(Buffer64);

            AcpiTableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) CurrentTableList->XenTablePhysicalAddress;

            //
            // If we find MADT, HPET, TCPA tables
            //
            if (!AsciiStrnCmp ((CHAR8 *) &AcpiTableHeader->Signature, "APIC", 4) ||
                !AsciiStrnCmp ((CHAR8 *) &AcpiTableHeader->Signature, "HPET", 4) ||
                !AsciiStrnCmp ((CHAR8 *) &AcpiTableHeader->Signature, "TCPA", 4)) {
              CurrentTableList->Signature = AcpiTableHeader->Signature;
              CurrentTableList->Length = AcpiTableHeader->Length;
            }
            else {
              //
              // For other tables found in secondary tables 
              //
              CurrentTableList->Signature = 0;
              if ((Index + 1) < Length) {
                CurrentTableList->Length = XsdtStructure->Entry[Index+1] - XsdtStructure->Entry[Index];
              }
              else{
                //
                // The last table ( We find the secondary tables is located directly adjacent to XSDT table in Xen ACPI)
                //
                CurrentTableList->Length = (UINTN) (XenAcpiTablePointerStructure->XenXsdtPtr) - XsdtStructure->Entry[Index];
              }
            }
            //
            // Add the table to the current list of tables
            //

            InsertTailList (&XenAcpiTablePointerStructure->XenSsdtTableList, &CurrentTableList->Link);
          }

          //
          // Detect the FADT 2.0, then get the DSDT and FACS 2.0
          // FADT 1.0 and FADT 2.0 point to the same DSDT and FACS 2.0 in Xen-4.2
          //
          Fadt2Structure = (EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *) (UINTN) XenAcpiTablePointerStructure->XenFadt2Ptr;

          if (!AsciiStrnCmp ((CHAR8 *) &Fadt2Structure->Header.Signature, "FACP", 4)) {

            Status = VerifyAcpiPlatformChecksum (
              Fadt2Structure,
              Fadt2Structure->Header.Length
              );

            if (Status != EFI_SUCCESS) {
              break;
            }

            XenAcpiTablePointerStructure->XenFacs2Ptr = (EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *) (UINTN) Fadt2Structure->FirmwareCtrl;
            XenAcpiTablePointerStructure->XenDsdt2Ptr = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Fadt2Structure->Dsdt;
          }
        }
      }
      else {

        //
        // RSDP 1.0 reserved.
        //
        if (Status != EFI_SUCCESS) {
          break;
        }
      }
    } //end RSDP
  }//end for

  return EFI_SUCCESS;
}

/**
  This function publishes the specified versions of the Xen ACPI tables by
  installing EFI configuration table entries for them. Any combination of
  table versions can be published.

  @param  AcpiTableInstance  Instance of the protocol.
  @param  Version            Version(s) to publish.

  @return EFI_SUCCESS  The function completed successfully.
  @return EFI_ABORTED  The function could not complete successfully.

**/
EFI_STATUS
EFIAPI
PublishTables (
  IN EFI_XEN_SUPPORT_ACPI_TABLE_INSTANCE      *AcpiTableInstance,
  IN EFI_ACPI_TABLE_VERSION                   Version
  )
{
  EFI_STATUS                Status;

  //
  // Do checksum again because Dsdt/Xsdt is updated.
  //
  ChecksumCommonTables (AcpiTableInstance);

  //
  // Add the RSD_PTR to the system table and store that we have installed the
  // tables.
  //
  if (((Version & EFI_ACPI_TABLE_VERSION_1_0B) != 0) &&
      !AcpiTableInstance->TablesInstalled1) {
    Status = gBS->InstallConfigurationTable (&gEfiAcpi10TableGuid, AcpiTableInstance->Rsdp1);
    if (EFI_ERROR (Status)) {
      return EFI_ABORTED;
    }

    AcpiTableInstance->TablesInstalled1 = TRUE;
  }

  if (((Version & EFI_ACPI_TABLE_VERSION_2_0) != 0 ||
       (Version & EFI_ACPI_TABLE_VERSION_3_0) != 0) &&
      !AcpiTableInstance->TablesInstalled2) {
    Status = gBS->InstallConfigurationTable (&gEfiAcpiTableGuid, AcpiTableInstance->Rsdp2);
    if (EFI_ERROR (Status)) {
      return EFI_ABORTED;
    }

    AcpiTableInstance->TablesInstalled2= TRUE;
  }

  return EFI_SUCCESS;
}

/**
  This function creates and adds Xen ACPI tables. 
  Create and add FADT 1.0 to RSDT;
  Create and add FADT 2.0 to XSDT;
  Create and add Secondary Tables(MADT, HPET, TCPA, etc) to RSDT and XSDT;
  Create and add DSDT and FACS to FACP.

  @param  AcpiTableInstance  Instance of the protocol.

  @return EFI_SUCCESS             The function completed successfully.

**/
EFI_STATUS
EFIAPI
SetAcpiTable (
  IN EFI_XEN_SUPPORT_ACPI_TABLE_INSTANCE   *AcpiTableInstance
  )
{
  EFI_STATUS                           Status;
  UINT32                               *CurrentRsdtEntry;
  VOID                                 *CurrentXsdtEntry;
  UINT64                               Buffer64;
  UINTN                                TotalSize;
  EFI_PHYSICAL_ADDRESS                 PageAddress;
  LIST_ENTRY                           *CurrentList;
  EFI_XEN_ACPI_SSDT_2_0_TABLE_LIST     *CurrentTable;
  UINTN                                RsdtEntryIndex;
  UINTN                                XsdtEntryIndex;

  //
  // Check for invalid input parameters
  //
  ASSERT (AcpiTableInstance);

  //
  // 1. Construct DSDT and FACS
  //
  if (XenAcpiTablePointerStructure->XenDsdt2Ptr) {
    //TotalSize = sizeof (XenAcpiTablePointerStructure->XenDsdt2Ptr->Length);
    TotalSize = XenAcpiTablePointerStructure->XenDsdt2Ptr->Length;
    PageAddress = 0xFFFFFFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiACPIReclaimMemory,
                    EFI_SIZE_TO_PAGES (TotalSize),
                    &PageAddress
                    );

    ZeroMem ((UINT8 *) (UINTN) PageAddress, TotalSize);

    AcpiTableInstance->Dsdt2 = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) PageAddress;

    //
    // Copy Xen DSDT contents to the new DSDT
    //
    CopyMem (
      AcpiTableInstance->Dsdt2,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenDsdt2Ptr,
      TotalSize
     );

    //
    // Checksum the table
    //
    AcpiPlatformChecksum (
      AcpiTableInstance->Dsdt2,
      AcpiTableInstance->Dsdt2->Length,
      OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER,
      Checksum)
      );
  }

  if (XenAcpiTablePointerStructure->XenFacs2Ptr) {
    TotalSize = sizeof (EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE);
    PageAddress = 0xFFFFFFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiACPIMemoryNVS,
                    EFI_SIZE_TO_PAGES (TotalSize),
                    &PageAddress
                    );

    ZeroMem ((UINT8 *) (UINTN) PageAddress, TotalSize);

    AcpiTableInstance->Facs2 = (EFI_ACPI_2_0_FIRMWARE_ACPI_CONTROL_STRUCTURE *) (UINTN) PageAddress;

    //
    // Copy Xen FACS 2.0 contents to the new FACS 2.0
    //
    CopyMem (
      AcpiTableInstance->Facs2,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenFacs2Ptr,
      TotalSize
     );

    //
    // Checksum the table
    //
    AcpiPlatformChecksum (
      AcpiTableInstance->Facs2,
      AcpiTableInstance->Facs2->Length,
      OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER,
      Checksum)
      );
  }

  //
  // 2. Set SSDT tables, and then add it to RSDT and XSDT entries
  //
  if (!IsListEmpty (&XenAcpiTablePointerStructure->XenSsdtTableList)) {
    RsdtEntryIndex = 0;
    XsdtEntryIndex = 0;
    CurrentList = &XenAcpiTablePointerStructure->XenSsdtTableList;

    do {
      CurrentList = GetNextNode(&XenAcpiTablePointerStructure->XenSsdtTableList, CurrentList);

      //
      // Create new table for every secondary table
      //
      CurrentTable = (EFI_XEN_ACPI_SSDT_2_0_TABLE_LIST *) (UINTN) CurrentList;

      TotalSize = CurrentTable->Length;
      PageAddress = 0xFFFFFFFF;
      Status = gBS->AllocatePages (
                      AllocateMaxAddress,
                      EfiACPIReclaimMemory,
                      EFI_SIZE_TO_PAGES (TotalSize),
                      &PageAddress
                      );

      ZeroMem ((UINT8 *) (UINTN) PageAddress, TotalSize);

      //
      // Copy Xen Acpi table contents to the new table
      //
      CopyMem (
        (VOID *) (UINTN) PageAddress,
        (VOID *) (UINTN) CurrentTable->XenTablePhysicalAddress,
        TotalSize
       );

      //
      // Checksum the table
      //
      AcpiPlatformChecksum (
        (VOID *) (UINTN) PageAddress,
        CurrentTable->Length,
        OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER,
        Checksum)
        );

      //
      // Add to RSDT 2.0
      //
      if (XenAcpiTablePointerStructure->XenFadt1Ptr && RsdtEntryIndex == 0) {
        //
        // If FADT 1.0 exists, we keep it the first one and add it later
        //
        RsdtEntryIndex = 1;
      }

      CurrentRsdtEntry  = (UINT32 *) ((UINT8 *) AcpiTableInstance->Rsdt2 + sizeof (EFI_ACPI_DESCRIPTION_HEADER) + RsdtEntryIndex * sizeof (UINT32));
      *CurrentRsdtEntry = (UINT32) (UINTN) PageAddress;

      //
      // Add to XSDT 2.0
      //
      if (XenAcpiTablePointerStructure->XenFadt1Ptr && XsdtEntryIndex == 0) {
        //
        // If FADT 2.0 exists, we keep it the first one and add it later
        //
        XsdtEntryIndex = 1;
      }

      CurrentXsdtEntry  = (VOID *) ((UINT8 *) AcpiTableInstance->Xsdt + sizeof (EFI_ACPI_DESCRIPTION_HEADER) + XsdtEntryIndex * sizeof (UINT64));
      //
      // Add entry to XSDT, XSDT expects 64 bit pointers, but
      // the table pointers in XSDT are not aligned on 8 byte boundary.
      //
      Buffer64 = (UINT64) (UINTN) PageAddress;
      CopyMem (
        CurrentXsdtEntry,
        &Buffer64,
        sizeof (UINT64)
        );

      RsdtEntryIndex++;
      XsdtEntryIndex++;

    } while (!IsNodeAtEnd(&XenAcpiTablePointerStructure->XenSsdtTableList, CurrentList));
  }

  //
  // 3. Set FADT 1.0, and then add it to RSDT
  //
  if (XenAcpiTablePointerStructure->XenFadt1Ptr) {
    TotalSize = sizeof (EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE);
    PageAddress = 0xFFFFFFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiACPIReclaimMemory,
                    EFI_SIZE_TO_PAGES (TotalSize),
                    &PageAddress
                    );

    ZeroMem ((UINT8 *) (UINTN) PageAddress, TotalSize);

    AcpiTableInstance->Fadt1 = (EFI_ACPI_1_0_FIXED_ACPI_DESCRIPTION_TABLE *) (UINTN) PageAddress;

    //
    // Copy Xen FADT 1.0 contents to the new FADT 1.0
    //
    CopyMem (
      AcpiTableInstance->Fadt1,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenFadt1Ptr,
      TotalSize
     );

    if (XenAcpiTablePointerStructure->XenFacs2Ptr) {
      AcpiTableInstance->Fadt1->FirmwareCtrl = (UINT32) (UINTN) AcpiTableInstance->Facs2;
    }
    if (XenAcpiTablePointerStructure->XenDsdt2Ptr) {
      AcpiTableInstance->Fadt1->Dsdt         = (UINT32) (UINTN) AcpiTableInstance->Dsdt2;
    }

    //
    // Checksum the table
    //
    AcpiPlatformChecksum (
      AcpiTableInstance->Fadt1,
      AcpiTableInstance->Fadt1->Header.Length,
      OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER,
      Checksum)
      );

    //
    // Add to RSDT 2.0
    //
    CurrentRsdtEntry  = (UINT32 *) ((UINT8 *) AcpiTableInstance->Rsdt2 + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
    *CurrentRsdtEntry = (UINT32) (UINTN) AcpiTableInstance->Fadt1;
  }

  //
  // 4. Set FADT 2.0, and then add it to XSDT
  //
  if (XenAcpiTablePointerStructure->XenFadt2Ptr) {
    TotalSize = sizeof (EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE);
    PageAddress = 0xFFFFFFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiACPIReclaimMemory,
                    EFI_SIZE_TO_PAGES (TotalSize),
                    &PageAddress
                    );

    ZeroMem ((UINT8 *) (UINTN) PageAddress, TotalSize);

    AcpiTableInstance->Fadt2 = (EFI_ACPI_2_0_FIXED_ACPI_DESCRIPTION_TABLE *) (UINTN) PageAddress;

    //
    // Copy Xen FADT 2.0 contents to the new FADT 2.0
    //
    CopyMem (
      AcpiTableInstance->Fadt2,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenFadt2Ptr,
      TotalSize
     );

    if (XenAcpiTablePointerStructure->XenFacs2Ptr) {
      AcpiTableInstance->Fadt2->FirmwareCtrl  = (UINT32) (UINTN) AcpiTableInstance->Facs2;
      AcpiTableInstance->Fadt2->XFirmwareCtrl = (UINT64) (UINTN) AcpiTableInstance->Facs2;
    }

    if (XenAcpiTablePointerStructure->XenDsdt2Ptr) {
      AcpiTableInstance->Fadt2->Dsdt          = (UINT32) (UINTN) AcpiTableInstance->Dsdt2;
      AcpiTableInstance->Fadt2->XDsdt         = (UINT64) (UINTN) AcpiTableInstance->Dsdt2;
    }

    //
    // Checksum the table
    //
    AcpiPlatformChecksum (
      AcpiTableInstance->Fadt2,
      AcpiTableInstance->Fadt2->Header.Length,
      OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER,
      Checksum)
      );

    //
    // Add to XSDT 2.0
    //
    CurrentXsdtEntry  = (VOID *) ((UINT8 *) AcpiTableInstance->Xsdt + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
    //
    // Add entry to XSDT, XSDT expects 64 bit pointers, but
    // the table pointers in XSDT are not aligned on 8 byte boundary.
    //
    Buffer64 = (UINT64) (UINTN) AcpiTableInstance->Fadt2;
    CopyMem (
      CurrentXsdtEntry,
      &Buffer64,
      sizeof (UINT64)
      );
  }

  return EFI_SUCCESS;
}

/**
  Entry point of the ACPI table driver.
  Creates and initializes an instance of the ACPI Table
  Protocol and installs it on a new handle.

  @param  ImageHandle   A handle for the image that is initializing this driver.
  @param  SystemTable   A pointer to the EFI system table.

  @return EFI_SUCCESS           Driver initialized successfully.
  @return EFI_LOAD_ERROR        Failed to Initialize or has been loaded.
  @return EFI_OUT_OF_RESOURCES  Could not allocate needed resources.

**/
EFI_STATUS
EFIAPI
XenAcpiTablePublish (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS            Status;
  UINTN                 TotalSize;
  UINT8                 *Pointer;
  EFI_PHYSICAL_ADDRESS  PageAddress;
  EFI_HOB_GUID_TYPE     *GuidHob;

  //
  // See if a XenInfo HOB is available
  //
  GuidHob = GetFirstGuidHob (&gEfiXenInfoGuid);
  if (GuidHob == NULL) {
    return EFI_SUCCESS;
  }

  //
  // Get Xen ACPI pointer: XEN_ACPI_TABLE_POINTER structure
  //
  XenAcpiTablePointerStructure = AllocateZeroPool (sizeof (XEN_ACPI_TABLE_POINTER));
  ASSERT (XenAcpiTablePointerStructure);

  InitializeXenAcpiTablePointer(XenAcpiTablePointerStructure);
  XenAcpiTableDetect(XenAcpiTablePointerStructure);

  //
  // We need to construct the AcpiTableInstance, and then publish Xen ACPI table
  // Initialize our protocol
  //
  AcpiTableInstance = AllocateZeroPool (sizeof (EFI_XEN_SUPPORT_ACPI_TABLE_INSTANCE));
  ASSERT (AcpiTableInstance);

  //
  // Create RSDP, RSDT, XSDT structures
  // Allocate all buffers
  //
  TotalSize = sizeof (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER) +
      sizeof (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER) +
      sizeof (EFI_ACPI_DESCRIPTION_HEADER) +      // for ACPI 1.0 RSDT
      mEfiAcpiMaxNumTables * sizeof (UINT32) +
      sizeof (EFI_ACPI_DESCRIPTION_HEADER) +      // for ACPI 2.0/3.0 RSDT
      mEfiAcpiMaxNumTables * sizeof (UINT32) +    // FADT 1.0, (SSDTs), ...
      sizeof (EFI_ACPI_DESCRIPTION_HEADER) +      // for ACPI 2.0/3.0 XSDT
      mEfiAcpiMaxNumTables * sizeof (UINT64);     // FADT 2.0, (SSDTs), ...

  //
  // Allocate memory in the lower 32 bit of address range for
  // compatibility with ACPI 1.0 OS.
  //
  // This is done because ACPI 1.0 pointers are 32 bit values.
  // ACPI 2.0 OS and all 64 bit OS must use the 64 bit ACPI table addresses.
  // There is no architectural reason these should be below 4GB, it is purely
  // for convenience of implementation that we force memory below 4GB.
  //
  PageAddress = 0xFFFFFFFF;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIReclaimMemory,
                  EFI_SIZE_TO_PAGES (TotalSize),
                  &PageAddress
                  );

  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  Pointer = (UINT8 *) (UINTN) PageAddress;
  ZeroMem (Pointer, TotalSize);

  AcpiTableInstance->Rsdp1 = (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER *) Pointer;
  Pointer += sizeof (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER);
  AcpiTableInstance->Rsdp2 = (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *) Pointer;
  Pointer += sizeof (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER);

  AcpiTableInstance->Rsdt1 = (EFI_ACPI_DESCRIPTION_HEADER *) Pointer;
  Pointer += (sizeof (EFI_ACPI_DESCRIPTION_HEADER) + EFI_ACPI_MAX_NUM_TABLES * sizeof (UINT32));
  AcpiTableInstance->Rsdt2 = (EFI_ACPI_DESCRIPTION_HEADER *) Pointer;
  Pointer += (sizeof (EFI_ACPI_DESCRIPTION_HEADER) + EFI_ACPI_MAX_NUM_TABLES * sizeof (UINT32));

  AcpiTableInstance->Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) Pointer;

  //
  // Initialize RSDP 1.0 by copying Xen ACPI RSDP 1.0 table, if it exists
  // (Not work for Xen now)
  //
  if (XenAcpiTablePointerStructure->XenRsdp1Ptr) {
    CopyMem (
      AcpiTableInstance->Rsdp1,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenRsdp1Ptr,
      sizeof (EFI_ACPI_1_0_ROOT_SYSTEM_DESCRIPTION_POINTER)
     );
     //
     // Set the RSDT pointer
     //
     AcpiTableInstance->Rsdp1->RsdtAddress = (UINT32) (UINTN) AcpiTableInstance->Rsdt1;
  }

  //
  // Initialize RSDP 2.0 by copying Xen ACPI RSDP 2.0 table
  //
  if (XenAcpiTablePointerStructure->XenRsdp2Ptr) {
    CopyMem (
      AcpiTableInstance->Rsdp2,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenRsdp2Ptr,
      sizeof (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER)
     );

     //
     // Set the RSDT pointer to new address
     //
     AcpiTableInstance->Rsdp2->RsdtAddress = (UINT32) (UINTN) AcpiTableInstance->Rsdt2;

     //
     // Set the XSDT pointer to new address
     //
     AcpiTableInstance->Rsdp2->XsdtAddress = (UINT64) (UINTN) AcpiTableInstance->Xsdt;
  }

  //
  // Initialize RSDT Header and Entries pointer (1.0/2.0)
  //
  if (XenAcpiTablePointerStructure->XenRsdt1Ptr) {
    //
    // There is no RSDT 1.0 in Xen ACPI (Not work for Xen now)
    //
    CopyMem (
      AcpiTableInstance->Rsdt1,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenRsdt1Ptr,
      sizeof (EFI_ACPI_DESCRIPTION_HEADER)
     );
  }
  if (XenAcpiTablePointerStructure->XenRsdt2Ptr) {
    //
    // Memory copy all the RSDT 2.0 entries of Xen RSDT
    //
    AcpiTableInstance->NumberOfTableEntries2 = XenAcpiTablePointerStructure->XenNumberOfTableEntries2;

    CopyMem (
      AcpiTableInstance->Rsdt2,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenRsdt2Ptr,
      sizeof (EFI_ACPI_DESCRIPTION_HEADER) + 
      AcpiTableInstance->NumberOfTableEntries2 * sizeof (UINT32)
     );

    //
    // Next we set the entry pointer to FADT, SSDTs in SetAcpiTable()
    //
  }

  //
  // Initialize XSDT Header and Entries pointer (1.0/2.0)
  //
  if (XenAcpiTablePointerStructure->XenXsdtPtr) {
    //
    // Memory copy all the RSDT entries of Xen RSDT
    //
    AcpiTableInstance->NumberOfTableEntries2 = XenAcpiTablePointerStructure->XenNumberOfTableEntries2;

    CopyMem (
      AcpiTableInstance->Xsdt,
      (VOID *) (UINTN) XenAcpiTablePointerStructure->XenXsdtPtr,
      sizeof (EFI_ACPI_DESCRIPTION_HEADER) + 
      AcpiTableInstance->NumberOfTableEntries2 * sizeof (UINT64)
     );

    //
    // Next we set the entry pointer to FADT, SSDTs in SetAcpiTable();
    //
  }

  Status = SetAcpiTable (AcpiTableInstance);

  if (!EFI_ERROR (Status)) {
    Status = PublishTables (
               AcpiTableInstance,
               EFI_ACPI_TABLE_VERSION_1_0B | EFI_ACPI_TABLE_VERSION_2_0 | EFI_ACPI_TABLE_VERSION_3_0
               );
  }

  //
  // Completed successfully
  //
  //return EFI_SUCCESS;
  return Status;
}

