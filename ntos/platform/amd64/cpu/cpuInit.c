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
#include <mm/phys.h>
#include <mm/vm.h>

#define ISR(fn) (ULONG_PTR)fn

static GDT_GDTR gdtr;
extern void lapic_tmr_isr(void);
extern void syscall_handler(void);

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
    kiIdtSetEntry(LAPIC_TMR_VEC, IDT_INT_GATE, ISR(lapic_tmr_isr), 0);
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
halInitSyscall(void)
{
    UQUAD msr;
    ULONG edx, unused;

    CPUID(0x80000001, unused, unused, unused, edx);
    if (!ISSET(edx, BIT(11))) {
        keBugCheck("SYSCALL/SYSRET not supported by CPU\n");
    }

    /* Enable the syscall instruction */
    msr = rdmsr(IA32_EFER);
    wrmsr(IA32_EFER, msr | 1);

    /* Write the STAR MSR */
    msr = ((UQUAD)USER_CS << 48) | ((UQUAD)KERNEL_CS << 32);
    wrmsr(IA32_STAR, msr);

    /* Write the handler to IA32_LSTAR */
    msr = (UQUAD)syscall_handler;
    wrmsr(IA32_LSTAR, msr);

    /* Disable interrupts on entry */
    msr = rdmsr(IA32_SFMASK);
    msr |= (1 << 9);
    wrmsr(IA32_SFMASK, msr);
}

void
halCpuInit(KPCR *kpcr)
{
    MCB *mcb;

    mcb = &kpcr->core;
    cpuInitGdt(&kpcr->core);
    TAILQ_INIT(&kpcr->queue.q);

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

    /* Allocate ourselves a system call stack */
    mcb->syscallStack = mmAllocFrame(1);
    if (mcb->syscallStack == 0) {
        keBugCheck("failed to allocate system call stack\n");
    }

    /* We want to start at the top, grows down */
    mcb->syscallStack += keGetKernelBase() + (PAGESIZE - 1);
}
