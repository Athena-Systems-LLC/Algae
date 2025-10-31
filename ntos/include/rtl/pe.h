/*
 * ALGAE portable executable definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _RTL_PE_H_
#define _RTL_PE_H_ 1

#include <ke/types.h>

/* Machine types */
#define IMAGE_FILE_MACHINE_UNKNOWN 0x0000
#define IMAGE_FILE_MACHINE_AMD64   0x8664

/* Optional header magic */
#define OPTHDR_MAG 0x20B

typedef struct _IMAGE_DOS_HEADER {      /* DOS .EXE header */
    USHORT e_magic;                     /* Magic number */
    USHORT e_cblp;                      /* Bytes on last page of file */
    USHORT e_cp;                        /* Pages in file */
    USHORT e_crlc;                      /* Relocations */
    USHORT e_cparhdr;                   /* Size of header in paragraphs */
    USHORT e_minalloc;                  /* Minimum extra paragraphs needed */
    USHORT e_maxalloc;                  /* Maximum extra paragraphs needed */
    USHORT e_ss;                        /* Initial (relative) SS value */
    USHORT e_sp;                        /* Initial SP value */
    USHORT e_csum;                      /* Checksum */
    USHORT e_ip;                        /* Initial IP value */
    USHORT e_cs;                        /* Initial (relative) CS value */
    USHORT e_lfarlc;                    /* File address of relocation table */
    USHORT e_ovno;                      /* Overlay number */
    USHORT e_res[4];                    /* Reserved words */
    USHORT e_oemid;                     /* OEM identifier (for e_oeminfo) */
    USHORT e_oeminfo;                   /* OEM information; e_oemid specific */
    USHORT e_res2[10];                  /* Reserved words */
    ULONG  e_lfanew;                    /* File address of new exe header */
} IMAGE_DOS_HEADER;

typedef struct _IMAGE_PE_HEADER {
    ULONG   e_magic;            /* PE\0\0 */
    USHORT  e_machine;          /* Machine type */
    USHORT  e_numsect;          /* Number of sections */
    ULONG   e_timestamp;        /* Timestamp */
    ULONG   e_symtabptr;        /* Symbol table pointer */
    ULONG   e_numsym;           /* Number of symbols */
    USHORT  e_opthdr_sz;        /* Optional header size */
    USHORT  e_flags;            /* Characteristics */
} IMAGE_PE_HEADER;

typedef struct _IMAGE_SECTION_HEADER {
    CHAR name[8];
    ULONG virtualSize;
    ULONG virtualAddr;
    ULONG rawDataSize;
    ULONG rawDataPtr;
    ULONG relocationsPtr;
    USHORT relocationsCount;
    USHORT lineNumbersCount;
    ULONG flags;
} IMAGE_SECTION_HEADER;

typedef struct _IMAGE_OPTIONAL_HEADER {
    USHORT magic;       /* 0x020B */
    UCHAR majorLinkerVersion;
    UCHAR minorLinkerVersion;
    ULONG sizeOfCode;
    ULONG sizeOfInitializedData;
    ULONG sizeOfUnitializeddata;
    ULONG addressOfEntryPoint;
    ULONG baseOfCode;
    UQUAD imageBase;
    ULONG sectionAlignment;
    ULONG fileAlignment;
    USHORT majorOperatingSystemVersion;
    USHORT minorOperatingSystemVersion;
    USHORT majorImageVersion;
    USHORT minorImageVersion;
    USHORT majorSubsystemVersion;
    USHORT minorSubsystemVersion;
    ULONG win32VersionValue;
    ULONG sizeOfImage;
    ULONG sizeOfHeaders;
    ULONG checkSum;
    USHORT subsystem;
    USHORT dllCharacteristics;
    UQUAD sizeOfStackReserve;
    UQUAD sizeOfStackCommit;
    UQUAD sizeOfHeapReserve;
    UQUAD sizeOfHeapCommit;
    ULONG loaderFlags;
    ULONG numberOfRvaAndSizes;
} IMAGE_OPTIONAL_HEADER;

#endif  /* !_RTL_PE_H_ */
