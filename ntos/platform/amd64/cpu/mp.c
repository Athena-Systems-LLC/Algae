/*
 * ALGAE multiprocessor support
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/defs.h>
#include <ke/kpcr.h>
#include <ke/timer.h>
#include <ke/bugCheck.h>
#include <ke/assert.h>
#include <ex/trace.h>
#include <md/lapic.h>
#include <md/mcb.h>
#include <rtl/string.h>
#include <acpi/acpi.h>
#include <hal/mmu.h>

#define TRAMPOLINE_PBASE 0x1000
#define TRAMPOLINE_LEN (__trampoline_end - __trampoline_start)

SECTION(".trampoline")
static UCHAR trampolineData[4096];

extern ULONG_PTR __trampoline_start;
extern UCHAR __trampoline_end;

static KTIMER *timer;
static UCHAR *trampoline;

/*
 * This function gets called per Local APIC as a callback
 * and initializes the application processors
 */
static int
localApicLookup(APIC_HEADER *hdr, USIZE arg)
{
    LAPIC_IPI ipi;
    LOCAL_APIC *lapic = (LOCAL_APIC *)hdr;
    NTSTATUS status;

    /*
     * The argument passed to us is the callers
     * processor ID, we use this to avoid blasting
     * ourselves with an INIT IPI
     */
    if (lapic->apicId == arg) {
        exTrace(EX_TRACE_INFO, "skip APIC %d\n", arg);
        return -1;
    }

    ipi.delmod = IPI_DELMOD_INIT;
    ipi.shand = IPI_SHAND_NONE;
    ipi.vector = 0;
    ipi.apicId = lapic->apicId;
    ipi.destMode = IPI_DESTMODE_PHYSICAL;

    /* Send the INIT IPI */
    status = kiLapicSendIpi(&ipi);
    if (status != STATUS_SUCCESS) {
        keBugCheck("could not send INIT IPI to APIC %d\n", ipi.apicId);
    }

    keTimerMsleep(timer, 10);
    ipi.delmod = IPI_DELMOD_STARTUP;
    ipi.vector = TRAMPOLINE_PBASE >> 12;

    /* Send a startup IPI */
    for (UCHAR i = 0; i < 2; ++i) {
        status = kiLapicSendIpi(&ipi);
        if (status != STATUS_SUCCESS) {
            keBugCheck("could not send STARTUP IPI (apic=%d)\n", ipi.apicId);
        }

        keTimerMsleep(timer, 1);
    }

    /* Return -1 to keep getting values */
    return -1;
}

void
kiMpInit(void)
{
    NTSTATUS status;
    MMU_VAS vas;
    UCHAR *p;
    KPCR *core;
    MCB *mcb;

    core = keGetCore();
    ASSERT(core != NULL);
    mcb = &core->core;

    /* Copy the trampoline data */
    trampoline = PHYS_TO_VIRT(TRAMPOLINE_PBASE);
    rtlMemcpy(trampoline, trampolineData, sizeof(trampolineData));

    /* Grab an aux timer */
    status = keTimerGetDescriptor(
        "/clkdev/aux",
        &timer
    );

    if (status != STATUS_SUCCESS) {
        keBugCheck("could not acquire aux timer object\n");
    }

    /* Get each processor */
    exTrace(EX_TRACE_INFO, "bringing up APs...\n");
    acpiReadMadt(
        APIC_TYPE_LOCAL_APIC,
        localApicLookup,
        mcb->hwId
    );
}
