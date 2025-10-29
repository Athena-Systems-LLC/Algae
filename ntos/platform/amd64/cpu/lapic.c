/*
 * ALGAE Local APIC driver
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/kpcr.h>
#include <ke/defs.h>
#include <ke/timer.h>
#include <ke/bugCheck.h>
#include <ke/assert.h>
#include <ke/kpcr.h>
#include <ob/object.h>
#include <ex/trace.h>
#include <ke/mmio.h>
#include <acpi/acpi.h>
#include <acpi/tables.h>
#include <md/tss.h>
#include <md/lapic.h>
#include <md/i8254.h>
#include <md/cpuid.h>
#include <md/mcb.h>
#include <md/msr.h>

/* LAPIC register offsets */
#define LAPIC_ID            0x0020U     /* ID Register */
#define LAPIC_VERSION       0x0030U     /* Version Register */
#define LAPIC_TPR           0x0080U     /* Task Priority Register */
#define LAPIC_APR           0x0090U     /* Arbitration Priority Register */
#define LAPIC_PPR           0x00A0U     /* Processor Priority Register */
#define LAPIC_EOI           0x00B0U     /* End Of Interrupt Register */
#define LAPIC_RRD           0x00C0U     /* Remote Read Register */
#define LAPIC_LDR           0x00D0U     /* Logical Destination Register */
#define LAPIC_DFR           0x00E0U     /* Destination Format Register */
#define LAPIC_SVR           0x00F0U     /* Spurious Vector Register */
#define LAPIC_ISR           0x0100U     /* In service register (max=0x0220) */
#define LAPIC_TMR           0x0180U     /* Trigger Mode Register (max=0x0220) */
#define LAPIC_IRR           0x0200U     /* Interrupt Request Register (max=0x0270) */
#define LAPIC_ERR           0x0280U     /* Error Status Register */
#define LAPIC_ICRLO         0x0300U     /* Interrupt Command Low Register */
#define LAPIC_ICRHI         0x0310U     /* Interrupt Command High Register */
#define LAPIC_LVT_TMR       0x0320U     /* LVT Timer Register */
#define LAPIC_DCR           0x03E0U     /* Divide Configuration Register (for timer) */
#define LAPIC_INIT_CNT      0x0380U     /* Initial Count Register (for timer) */
#define LAPIC_CUR_CNT       0x0390U     /* Current Count Register (for timer) */

#define LAPIC_TMR_SAMPLES 0xFFFF
#define X2APIC_IPI_SELF 0x3F0

/*
 * The x2APIC register space is accessed via
 * RDMSR/WRMSR instructions. The below defines
 * the base MSR address for the register space.
 */
#define x2APIC_MSR_BASE 0x00000800

/*
 * To hardware enable, OR the value of the IA32_APIC_BASE
 * MSR with LAPIC_HW_ENABLE and rewrite it.
 *
 * To software enable, OR the value of the SVR with
 * LAPIC_SW_ENABLE and rewrite it.
 *
 * LAPIC_SW_ENABLE has the low 8 bits set as some hardware
 * requires the spurious vector to be hardwired to 1s so
 * we'll go with that to be safe.
 */
#define LAPIC_HW_ENABLE     BIT(11)
#define LAPIC_SW_ENABLE     (BIT(8) | 0xFF)
#define x2APIC_ENABLE_SHIFT 10

/* LVT bits */
#define LAPIC_LVT_MASK            BIT(16)
#define LVT_TMR_ONESHOT           0x00
#define LVT_TMR_PERIODIC          0x01
#define LVT_TMR_TSC_DEADLINE      0x02

/* MMIO base address (relocatable) */
#define LAPIC_MMIO_BASE 0xFEE00000

static KTIMER ktimer;
static NT_OBJECT *clkdev;

/*
 * Section 10.4.2 of the Intel SDM states that the
 * presense of an on-chip Local APIC unit can be
 * detected via CPUID(1).EAX[bit 9]
 */
static inline BOOLEAN
lapicIsPresent(void)
{
    ULONG edx, unused;

    CPUID(0x01, unused, unused, unused, edx);
    return ISSET(edx, BIT(9)) != 0;
}

/*
 * Section 10.12.1 of the Intel SDM states that the Local APIC
 * unit can enter x2APIC mode if CPUID(1).ECX[bit 21] is set
 */
static inline BOOLEAN
lapicHasX2Apic(void)
{
    ULONG ecx, unused;

    CPUID(0x01, unused, unused, ecx, unused);
    return ISSET(ecx, BIT(21)) != 0;
}

/*
 * Read a value from a Local APIC register
 */
static UQUAD
lapicRead(const MCB *core, ULONG reg)
{
    void *addr;

    /*
     * x2APIC mode uses model specific registers instead
     * of MMIO like the old xAPIC mode
     */
    if (core->hasX2Apic) {
        return rdmsr(x2APIC_MSR_BASE + (reg >> 4));
    }

    addr = PTR_OFFSET(core->lapicBase, reg);
    return mmioRead32(addr);
}

/*
 * Write a value to a Local APIC register
 */
static void
lapicWrite(const MCB *core, ULONG reg, UQUAD value)
{
    void *addr;

    if (core->hasX2Apic) {
        wrmsr(x2APIC_MSR_BASE + (reg >> 4), value);
        return;
    }

    addr = PTR_OFFSET(core->lapicBase, reg);
    mmioWrite32(addr, value);
}

/*
 * Set the MMIO base for the Local APIC using
 * ACPI
 */
static void
lapicSetBase(KPCR *kpcr)
{
    static ACPI_MADT *madt = NULL;
    MCB *mcb;

    ASSERT(kpcr != NULL);

    /*
     * We'll only need to set this once as we'll cache
     * it to avoid the lookup per core
     */
    if (madt == NULL) {
        madt = acpiQueryTable("APIC");
    }

    /* If we don't have it, bail */
    if (madt == NULL) {
        keBugCheck("lapicSetBase: could not query APIC tables\n");
    }

    /*
     * Typically the Local APIC base should be at LAPIC_MMIO_BASE,
     * however we cannot be so sure that firmware hasn't relocated it
     * and we currently don't support handling of that.
     */
    if (madt->lapicAddr != LAPIC_MMIO_BASE) {
        keBugCheck("lapicSetBase: detected relocation of LAPIC base\n");
    }

    mcb = &kpcr->core;
    mcb->lapicBase = madt->lapicAddr;
}

/*
 * Returns the local APIC ID assigned at power
 * up
 */
static ULONG
lapicReadId(MCB *core)
{
    ULONG idReg;

    if (core == NULL) {
        return 0;
    }

    /*
     * In xAPIC mode, the APIC ID is an 8 bit value
     */
    idReg = lapicRead(core, LAPIC_ID);
    if (!core->hasX2Apic) {
        return (idReg >> 24) & 0xFF;
    }

    /*
     * In x2APIC mode, the APIC ID is a 32-bit value
     */
    return lapicRead(core, LAPIC_ID);
}

/*
 * Enable the Local APIC unit
 */
static void
lapicEnable(MCB *core)
{
    const char *mode = "xapic";
    UQUAD apicMsr;
    UQUAD svr;

    core->hasX2Apic = lapicHasX2Apic();

    /*
     * Hardware enable it, and in x2APIC mode if
     * we can
     */
    apicMsr = rdmsr(IA32_APIC_BASE_MSR);
    apicMsr |= LAPIC_HW_ENABLE;
    apicMsr |= core->hasX2Apic << 10;
    wrmsr(IA32_APIC_BASE_MSR, apicMsr);

    /* Software enable the Local APIC */
    svr = lapicRead(core, LAPIC_SVR);
    svr |= LAPIC_SW_ENABLE;
    lapicWrite(core, LAPIC_SVR, svr);

    core->hwId = lapicReadId(core);
    if (core->hasX2Apic) {
        mode = "x2apic";
    }

    exTrace(
        EX_TRACE_INFO,
        "lapic[%d]: local APIC unit enabled in %s mode\n",
        core->hwId,
        mode
    );
}

/*
 * Stop and mask the Local APIC timer
 */
static void
lapicStopTimer(MCB *core)
{
    lapicWrite(core, LAPIC_LVT_TMR, LAPIC_LVT_MASK);
    lapicWrite(core, LAPIC_INIT_CNT, 0);
}

static void
lapicTimerStart(MCB *core, BOOLEAN mask, UCHAR mode, ULONG count)
{
    ULONG tmp;

    tmp = (mode << 17) | (mask << 16) | LAPIC_TMR_VEC;
    lapicWrite(core, LAPIC_LVT_TMR, tmp);
    lapicWrite(core, LAPIC_DCR, 0x00);
    lapicWrite(core, LAPIC_INIT_CNT, count);
}

/*
 * Initialize the Local APIC timer
 */
static USIZE
lapicTimerInit(MCB *core)
{
    USHORT ticksStart, ticksEnd;
    USIZE ticksTotal, frequency;

    lapicStopTimer(core);
    pitSetCount(LAPIC_TMR_SAMPLES);
    ticksStart = pitGetCount();

    /* Begin sampling */
    lapicWrite(core, LAPIC_INIT_CNT, LAPIC_TMR_SAMPLES);
    while (lapicRead(core, LAPIC_CUR_CNT) > 0);

    /* Compute the total number of ticks */
    ticksEnd = pitGetCount();
    ticksTotal = (ticksStart - ticksEnd);

    frequency = (LAPIC_TMR_SAMPLES / ticksTotal) * I8254_DIVIDEND;
    lapicStopTimer(core);
    return frequency;
}

/*
 * Start Local APIC timer oneshot with number
 * of ticks to count down from.
 *
 * @mask: If `true', timer will be masked, `count' should be 0.
 * @count: Number of ticks.
 */
static void
lapicTimerOneshot(MCB *core, BOOLEAN mask, USIZE count)
{
    USIZE ticksStart, ticksEnd;
    USIZE ticksTotal, frequency;

    lapicTimerStart(core, mask, LVT_TMR_ONESHOT, count);
}

/*
 * Perform a timer oneshot using microsecond units
 */
static void
lapicTimerOneshotUsec(struct ktimer *ktimer, USIZE usec)
{
    USIZE ticks;
    KPCR *kpcr;
    MCB *mcb;

    kpcr = keGetCore();
    if (kpcr == NULL) {
        keBugCheck("could not get current core\n");
    }

    mcb = &kpcr->core;
    ticks = usec * (mcb->lapicTmrFreq / 1000000);
    lapicTimerOneshot(mcb, false, ticks);
}

void
kiLapicInitTimer(void)
{
    static BOOLEAN isInit = false;
    NTSTATUS status;
    NT_OBJECT_CREATE args;

    if (isInit) {
        return;
    }

    args.parent = "/clkdev";
    args.name = "sched";
    args.type = NT_OB_TIMER;
    status = obCreateObject(&args, &clkdev);
    if (status != STATUS_SUCCESS) {
        keBugCheck("could not register sched timer\n");
    }

    clkdev->data = &ktimer;
    isInit = true;
}

NTSTATUS
kiLapicSendIpi(const LAPIC_IPI *ipi)
{
    KPCR *kpcr = keGetCore();
    MCB *mcb;
    ULONG icrLo = 0, icrHi = 0;

    if (kpcr == NULL) {
        keBugCheck("could not get current core\n");
    }

    mcb = &kpcr->core;

    /* Use optimized self shorthand if x2APIC */
    if (ipi->shand == IPI_SHAND_SELF && mcb->hasX2Apic) {
        lapicWrite(mcb, X2APIC_IPI_SELF, ipi->vector);
        return STATUS_SUCCESS;
    }

    /* Encode the low dword */
    icrLo |= ipi->vector;
    icrLo |= (ipi->delmod & 0x7) << 8;
    icrLo |= (ipi->destMode & 0x1) << 11;
    icrLo |= (ipi->shand & 0x3) << 18;

    /* Polling is redundant on x2APIC as it queues */
    if (mcb->hasX2Apic) {
        lapicWrite(
            mcb,
            LAPIC_ICRLO,
            ((UQUAD)ipi->apicId) << 32 | icrLo
        );

        return STATUS_SUCCESS;
    }

    /* Send that IPI */
    lapicWrite(mcb, LAPIC_ICRHI, ((ULONG)ipi->apicId << 24));
    lapicWrite(mcb, LAPIC_ICRLO, icrLo);

    /* Manual serialization is required on xAPIC */
    do {
        icrLo = lapicRead(mcb, LAPIC_ICRLO);
        ASMV("pause");
    } while (ISSET(icrLo, BIT(12)));
    return STATUS_SUCCESS;
}

void
kiLapicInit(void)
{
    TSS_STACK tmrStack;
    NTSTATUS status;
    KPCR *kpcr = keGetCore();
    MCB *core = &kpcr->core;

    /*
     * Currently, ALGAE relies on the presense of a
     * local APIC on every processor.
     */
    if (!lapicIsPresent()) {
        keBugCheck("CPU does not include on-board LAPIC unit\n");
    }

    /* Enable the APIC unit */
    lapicEnable(core);

    /* Calibrate the built-in timer */
    core->lapicTmrFreq = lapicTimerInit(core);

    /* Allocate a stack for the timer interrupts */
    status = tssAllocateStack(&tmrStack);
    if (status != STATUS_SUCCESS) {
        keBugCheck("could not allocate sched timer stack\n");
    }

    /* Set the timer stack */
    status = tssSetIntrStack(kpcr, IST_SCHED, &tmrStack);
    if (status != STATUS_SUCCESS) {
        keBugCheck("could not set sched timer stack\n");
    }
}

static KTIMER ktimer = {
    .oneshotUsec = lapicTimerOneshotUsec
};
