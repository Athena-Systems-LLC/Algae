/*
 * ALGAE Machine Core Block (MD processor information)
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_MCB_H_
#define _MACHINE_MCB_H_ 1

#include <ke/types.h>
#include <ke/defs.h>
#include <md/gdt.h>

#define MD_SPINWAIT() \
    ASMV("rep; nop")

/*
 * The machine core block is architecture dependent and
 * contains MD specific information about a processor
 * core
 *
 * @lapicBase: Virtual Local APIC MMIO base address
 * @hwId: Local APIC ID
 * @hasX2Apic: Supports x2APIC mode if true
 * @gdt: Global descriptor table
 * @gdtr: Global descriptor table register
 */
typedef struct {
    ULONG_PTR lapicBase;
    USHORT hwId;
    BOOLEAN hasX2Apic;
    GDT_ENTRY gdt[GDT_ENTRY_COUNT];
    GDT_GDTR gdtr;
} MCB;

#endif  /* !_MACHINE_MCB_H_ */
