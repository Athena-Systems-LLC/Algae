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
#include <ke/spinlock.h>
#include <ke/boot.h>
#include <ex/trace.h>
#include <ex/pool.h>
#include <ke/boot.h>
#include <md/lapic.h>
#include <md/mcb.h>
#include <rtl/string.h>
#include <acpi/acpi.h>
#include <mm/phys.h>
#include <hal/mmu.h>
#include <hal/process.h>
#include <hal/kpcr.h>

#define trace(...) \
    exTrace(EX_TRACE_INFO, "mp: " __VA_ARGS__)

#define AP_MAX 256
#define TRAMPOLINE_PBASE 0x1000
#define BOOTSTRAP_DESC_BASE 0x2000
#define TRAMPOLINE_LEN (__trampoline_end - __trampoline_start)

SECTION(".trampoline")
static UCHAR trampolineData[4096];

extern ULONG_PTR __trampoline_start;
extern UCHAR __trampoline_end;

static KPCR *bspKpcr;
static KPCR *apList[AP_MAX];
static KSPIN_LOCK apLock;
static KTIMER *timer;
static UCHAR *trampoline;
static USIZE nAp = 0;

/*
 * A bootstrap descriptor contains information needed
 * by the application processor to transition into
 * long mode.
 *
 * @pml4Pa: Physical address that will end up in CR3
 * @entryPa: Entry physical address for AP
 * @stackBase: Base stack address
 *
 * XXX: DO NOT REORDER - CALLED IN ASSEMBLY
 */
typedef struct {
    ULONG_PTR pml4Pa;
    ULONG_PTR entryPa;
    ULONG_PTR stackBase;
} AP_BOOTSTRAP;

static void
mpApEntry(void)
{
    KPCR *kpcr;

    keAcquireSpinLock(&apLock);

    kpcr = exAllocatePool(POOL_NON_PAGED, sizeof(KPCR));
    ASSERT(kpcr != NULL);

    apList[nAp++] = kpcr;
    ASSERT(nAp < AP_MAX);

    TAILQ_INIT(&kpcr->queue.q);
    kiProcessorInit(kpcr);
    kiLapicInitTimer();
    keReleaseSpinLock(&apLock);

    ASMV("sti");
    halSchedEnter();
}

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
    AP_BOOTSTRAP *bootstrap;
    MMU_VAS vas;

    /*
     * The argument passed to us is the callers
     * processor ID, we use this to avoid blasting
     * ourselves with an INIT IPI
     */
    if (lapic->apicId == arg) {
        trace("skip APIC %d\n", arg);
        return -1;
    }

    /* Initialize the bootstrap descriptor */
    mmuReadVas(&vas);
    bootstrap = PHYS_TO_VIRT(BOOTSTRAP_DESC_BASE);
    bootstrap->pml4Pa = vas.cr3;
    bootstrap->entryPa = (ULONG_PTR)mpApEntry;
    bootstrap->stackBase = mmAllocFrame(1);
    ASSERT(bootstrap->stackBase != 0);

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

        keTimerMsleep(timer, 5);
    }

    /* Return -1 to keep getting values */
    return -1;
}

USIZE
halCpuCount(void)
{
    return nAp + 1;
}

NTSTATUS
halCpuGet(USHORT cpuIdx, struct kpcr **result)
{
    if (result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (cpuIdx == 0) {
        *result = bspKpcr;
        return STATUS_SUCCESS;
    }

    *result = apList[cpuIdx - 1];
    return STATUS_SUCCESS;
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

    bspKpcr = core;
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
    trace("bringing up APs...\n");
    acpiReadMadt(
        APIC_TYPE_LOCAL_APIC,
        localApicLookup,
        mcb->hwId
    );

    trace("bootstrapped %d cores\n", nAp);
}
