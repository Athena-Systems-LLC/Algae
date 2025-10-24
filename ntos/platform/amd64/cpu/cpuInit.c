/*
 * ALGAE MD CPU initialization
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/cpu.h>
#include <ke/types.h>
#include <md/idt.h>
#include <md/gdt.h>
#include <md/idt.h>
#include <md/trap.h>

#define ISR(fn) (ULONG_PTR)fn

static struct gdtr gdtr;

static void
halRegisterIntr(void)
{
    kiIdtSetEntry(0x0, IDT_TRAP_GATE, ISR(arith_err), 0);
    kiIdtSetEntry(0x2, IDT_TRAP_GATE, ISR(nmi), 0);
    kiIdtSetEntry(0x3, IDT_TRAP_GATE, ISR(breakpoint_handler), 0);
    kiIdtSetEntry(0x4, IDT_TRAP_GATE, ISR(overflow), 0);
    kiIdtSetEntry(0x5, IDT_TRAP_GATE, ISR(bound_range), 0);
    kiIdtSetEntry(0x6, IDT_TRAP_GATE, ISR(invl_op), 0);
    kiIdtSetEntry(0x8, IDT_TRAP_GATE, ISR(double_fault), 0);
    kiIdtSetEntry(0xA, IDT_TRAP_GATE, ISR(invl_tss), 0);
    kiIdtSetEntry(0xB, IDT_TRAP_GATE, ISR(segnp), 0);
    kiIdtSetEntry(0xC, IDT_TRAP_GATE, ISR(ss_fault), 0);
    kiIdtSetEntry(0xD, IDT_TRAP_GATE, ISR(general_prot), 0);
    kiIdtSetEntry(0xE, IDT_TRAP_GATE, ISR(page_fault), 0);
}

void
halCpuInit(void)
{
    gdtr.offset = (ULONG_PTR)&g_gdtData[0];
    gdtr.limit = sizeof(g_gdtData) - 1;
    kiGdtLoad(&gdtr);

    /* Enable interrupts */
    kiIdtLoad();
    halRegisterIntr();
}
