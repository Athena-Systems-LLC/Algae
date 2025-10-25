/*
 * ALGAE ACPI table definitions.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _ACPI_TABLES_H_
#define _ACPI_TABLES_H_

#include <ke/types.h>
#include <ke/defs.h>

#define APIC_TYPE_LOCAL_APIC            0
#define APIC_TYPE_IO_APIC               1
#define APIC_TYPE_INTERRUPT_OVERRIDE    2
#define OEMID_SIZE 6

typedef struct PACKED {
    CHAR signature[4];        /* ASCII signature string */
    ULONG length;             /* Length of table in bytes */
    UCHAR revision;           /* Revision of the structure */
    UCHAR checksum;           /* Checksum of the header */
    CHAR oemid[OEMID_SIZE];   /* OEM-supplied string that IDs the OEM */
    CHAR oemTblId[8];         /* OEM-supplied string (used by OEM) */
    ULONG oemRev;             /* OEM-supplied revision number */
    ULONG creatorId;          /* Vendor ID of creator utility */
    ULONG creatorRev;         /* Revision of creator utility */
} ACPI_HEADER;

typedef struct PACKED {
    UQUAD signature;          /* RSD PTR */
    UCHAR checksum;           /* Structure checksum */
    CHAR oemid[OEMID_SIZE];   /* OEM-supplied string that IDs the OEM */
    UCHAR revision;           /* Revision of the structure */

    ULONG rsdtAddr;          /* RSDT physical address */

    /* Reserved if revision < 2 */
    ULONG length;             /* Length of table in bytes */
    UQUAD xsdtAddr;          /* XSDT physical address */
    UCHAR extCsum;           /* Extended checksum */
    UCHAR reserved[3];
} ACPI_RSDP;

typedef struct PACKED {
    ACPI_HEADER hdr;
    ULONG table[];
} ACPI_ROOT_SDT;

#endif  /* !_ACPI_TABLES_H_ */
