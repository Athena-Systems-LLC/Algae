/*
 * ALGAE HPET timer driver
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/defs.h>
#include <ke/mmio.h>
#include <ke/timer.h>
#include <ke/bugCheck.h>
#include <ob/object.h>
#include <ex/trace.h>
#include <acpi/tables.h>
#include <acpi/acpi.h>
#include <mm/vm.h>
#include <ntstatus.h>

/* HPET register offsets */
#define HPET_REG_CAPS               0x00
#define HPET_REG_CAPS               0x00
#define HPET_GENERAL_CONFIG         0x10
#define HPET_REG_MAIN_COUNTER       0xF0

#define CAP_REV_ID(caps)            (caps & 0xFF)
#define CAP_NUM_TIM(caps)           (caps >> 8) & 0x1F
#define CAP_CLK_PERIOD(caps)        (caps >> 32)

static KTIMER timer;
static NT_OBJECT *clkdev = NULL;
static void *hpetBase = NULL;

/*
 * Read from a 64-bit HPET register
 *
 * @reg: Register to read from
 */
static UQUAD
hpetRead(USHORT reg)
{
    UQUAD *regBase = PTR_OFFSET(hpetBase, reg);

    return mmioRead64(regBase);
}

/*
 * Write a 64-bit value to an HPET register
 */
static void
hpetWrite(USHORT reg, UQUAD val)
{
    UQUAD *regBase = PTR_OFFSET(hpetBase, reg);

    mmioWrite64(regBase, val);
}

/*
 * Sleep for 'n' amount of 'units'
 *
 * @n: Number of 'units' to sleep
 * @units: Units per 'n'
 *
 * Returns zero on success, otherwise a less than zero
 * value.
 */
static NTSTATUS
hpetSleep(UQUAD n, UQUAD units)
{
    UQUAD period;
    UQUAD caps;
    UQUAD counter;
    USIZE ticks;

    caps = hpetRead(HPET_REG_CAPS);
    period = CAP_CLK_PERIOD(caps);
    counter = hpetRead(HPET_REG_MAIN_COUNTER);
    ticks = counter + (n * (units / period));

    while (hpetRead(HPET_REG_MAIN_COUNTER) < ticks) {
        ASMV("pause");
    }

    return STATUS_SUCCESS;
}

static NTSTATUS
hpetRegister(void)
{
    NTSTATUS status;
    NT_OBJECT_CREATE params;

    params.parent = "/clkdev";
    params.name = "aux";
    params.type = NT_OB_TIMER;
    status = obCreateObject(&params, &clkdev);
    if (status != STATUS_SUCCESS) {
        return status;
    }

    clkdev->data = &timer;
    return STATUS_SUCCESS;
}

static NTSTATUS
hpetMsleep(USIZE ms)
{
    return hpetSleep(ms, 1000000000000);
}

static NTSTATUS
ktimerMSleep(KTIMER *timer, USIZE ms)
{
    return hpetMsleep(ms);
}

void
kiHpetInit(void)
{
    NTSTATUS status;
    ACPI_HPET *hpet;
    ACPI_GAS *gas;
    UQUAD value;
    ULONG clkPeriod;
    UCHAR revId;

    /* HPET is required */
    hpet = acpiQueryTable("HPET");
    if (hpet == NULL) {
        keBugCheck("board does not include HPET\n");
    }

    /* Grab the address base */
    gas = &hpet->gas;
    hpetBase = PHYS_TO_VIRT(gas->address);
    exTrace(EX_TRACE_INFO, "hpet: HPET detected - verifying integrity...\n");

    /* Grab values from general capabilities */
    value = hpetRead(HPET_REG_CAPS);
    clkPeriod = CAP_CLK_PERIOD(value);
    revId = CAP_REV_ID(value);

    /*
     * Clock period must NOT be zero and its maximum value
     * according to the spec is 05F5E100h
     */
    if (clkPeriod == 0 || clkPeriod > 0x05F5E100) {
        keBugCheck("bad HPET COUNTER_CLK_PERIOD\n");
    }

    /*
     * The revision ID must not be zero according to
     * the spec.
     */
    if (revId == 0) {
        keBugCheck("bad HPET revision ID\n");
    }

    /* Clear the counter and enable the timer */
    hpetWrite(HPET_REG_MAIN_COUNTER, 0);
    hpetWrite(HPET_GENERAL_CONFIG, 1);

    /* Register */
    status = hpetRegister();
    if (status != STATUS_SUCCESS) {
        keBugCheck("could not register HPET\n");
    }
}

static KTIMER timer = {
    .msleep = ktimerMSleep
};
