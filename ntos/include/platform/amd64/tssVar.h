/*
 * ALGAE Task state segment
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_TSSVAR_H_
#define _MACHINE_TSSVAR_H_

#include <ke/types.h>

/*
 * The 64-bit task state segment entry
 *
 * See Intel SDM Section 8.2.1
 */
typedef struct PACKED {
    ULONG reserved1;
    ULONG rsp0Lo;
    ULONG rsp0Hi;
    ULONG rsp1Lo;
    ULONG rsp1Hi;
    ULONG rsp2Lo;
    ULONG rsp2Hi;
    UQUAD reserved2;
    ULONG ist1Lo;
    ULONG ist1Hi;
    ULONG ist2Lo;
    ULONG ist2Hi;
    ULONG ist3Lo;
    ULONG ist3Hi;
    ULONG ist4Lo;
    ULONG ist4Hi;
    ULONG ist5Lo;
    ULONG ist5Hi;
    ULONG ist6Lo;
    ULONG ist6Hi;
    ULONG ist7Lo;
    ULONG ist7Hi;
    UQUAD reserved3;
    USHORT reserved4;
    USHORT iobase;
    UCHAR iomap[8192];
} TSS_ENTRY;

/*
 * 64-bit TSS descriptor
 *
 * See Intel SDM Section 8.2.3
 */
typedef struct PACKED {
    USHORT seglimit;
    USHORT baseLo16;
    UCHAR baseMid8;
    UCHAR type        : 4;
    UCHAR zero        : 1;
    UCHAR dpl         : 2;
    UCHAR p           : 1;
    UCHAR seglimitHi  : 4;
    UCHAR avl         : 1;
    UCHAR unused      : 2;
    UCHAR g           : 1;
    UCHAR baseHiMid8;
    ULONG baseHi32;
    ULONG reserved;
} TSS_DESC;

#endif  /* !_MACHINE_TSSVAR_H_ */
