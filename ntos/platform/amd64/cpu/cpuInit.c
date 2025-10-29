/*
 * ALGAE MD CPU initialization
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/cpu.h>
#include <ke/kpcr.h>
#include <ke/types.h>
#include <ke/defs.h>
#include <hal/irql.h>
#include <rtl/string.h>
#include <md/tss.h>
#include <md/lapic.h>
#include <md/msr.h>
#include <md/idt.h>
#include <md/gdt.h>
#include <md/idt.h>
#include <md/trap.h>

#define ISR(fn) (ULONG_PTR)fn

static GDT_GDTR gdtr;

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

static void
cpuInitGdt(MCB *mcb)
{
    GDT_GDTR *gdtr;

    if (mcb == NULL) {
        return;
    }

    rtlMemcpy(mcb->gdt, g_gdtData, sizeof(g_gdtData));
    gdtr = &mcb->gdtr;
    gdtr->limit = sizeof(g_gdtData) - 1;
    gdtr->offset = (ULONG_PTR)&mcb->gdt[0];
    kiGdtLoad(gdtr);
}

static void
cpuInitTss(KPCR *kpcr)
{
    MCB *mcb;

    if (kpcr == NULL) {
        return;
    }

    mcb = &kpcr->core;
    tssWrite(kpcr, (TSS_DESC *)&mcb->gdt[GDT_TSS_INDEX]);
    tssLoad();
}

void
halCpuHalt(CPU_HALT_MODE mode)
{
    for (;;) {
        ASMV("cli; hlt");
    }
}

KPCR *
keGetCore(void)
{
    KPCR *kpcr;

    if (rdmsr(IA32_GS_BASE) == 0) {
        return NULL;
    }

    /* Use self field to get core */
    ASMV(
        "mov %%gs:%1, %0"
        : "=r" (kpcr)
        : "m" (*&((KPCR *)0)->self)
    );

    return kpcr;
}

void
halCpuInit(KPCR *kpcr)
{
    cpuInitGdt(&kpcr->core);

    /* Enable interrupts */
    kiIdtLoad();
    halRegisterIntr();

    /* Load the TSS */
    cpuInitTss(kpcr);

    /* Set the current processor */
    kpcr->self = kpcr;
    wrmsr(IA32_GS_BASE, (ULONG_PTR)kpcr);

    /* Enable the Local APIC unit */
    kiLapicInit();

    /* Put ourselves at the passive level */
    halSetIrql(PASSIVE_LEVEL);
}
