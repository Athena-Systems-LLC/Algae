/*
 * ALGAE AMD64 IDT definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_IDT_H_
#define _MACHINE_IDT_H_ 1

#include <ke/types.h>
#include <ke/defs.h>

#define IDT_INT_GATE    0x8E
#define IDT_TRAP_GATE   0x8F
#define IDT_USER_GATE   0xEE

/*
 * Interrupt gate descriptor
 */
struct idtGate {
    USHORT offsetLow;
    USHORT codeSeg;
    UCHAR ist   : 3;
    UCHAR zero  : 5;
    UCHAR type  : 4;
    UCHAR zero1 : 1;
    UCHAR dpl   : 2;
    UCHAR p     : 1;
    USHORT offsetMid;
    ULONG offsetHigh;
    ULONG reserved;
} PACKED;

/*
 * Interrupt descriptor table register
 */
struct idtr {
    USHORT limit;
    ULONG_PTR offset;
} PACKED;

/*
 * Load the interrupt descriptor table
 */
void kiIdtLoad(void);

/*
 * Set an interrupt descriptor table entry
 *
 * @vec: Vector to set
 * @type: Interrupt gate type
 * @isr: Interrupt service routine address
 * @ist: Interrupt stack table entry
 */
void kiIdtSetEntry(UCHAR vec, UCHAR type, ULONG_PTR isr, UCHAR ist);

#endif  /* !_MACHINE_IDT_H_ */
