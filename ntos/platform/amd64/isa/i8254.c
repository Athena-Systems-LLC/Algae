/*
 * ALGAE Intel i8254 driver
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/timer.h>
#include <hal/pio.h>
#include <ob/object.h>
#include <md/i8254.h>

static BOOLEAN isInit = false;
static NT_OBJECT *clkdev;
static KTIMER timer;

static USIZE
ktimerGetCount(struct ktimer *tmr)
{
    return pitGetCount();
}

static void
ktimerSetCount(struct ktimer *tmr, USIZE count)
{
    pitSetCount(count);
}

USHORT pitGetCount(void)
{
    UCHAR lo, hi;

    halBusOutb(I8254_COMMAND, 0x00);
    lo = halBusInb(I8254_CHANNEL_0);
    hi = halBusInb(I8254_CHANNEL_0);
    return (hi << 8) | lo;
}

void
pitSetCount(USHORT count)
{
    halBusOutb(I8254_COMMAND, 0x34);
    halBusOutb(I8254_CHANNEL_0, count & 0xFF);
    halBusOutb(I8254_CHANNEL_0, (count >> 8) & 0xFF);
}

void
pitSetFrequency(UQUAD frequencyHz)
{
    UQUAD divisor = I8254_DIVIDEND / frequencyHz;

    if ((I8254_DIVIDEND % frequencyHz) > (frequencyHz / 2)) {
        ++divisor;
    }

    pitSetCount(divisor);
}

NTSTATUS
pitInit(void)
{
    NT_OBJECT_CREATE create;
    NTSTATUS status;

    if (isInit) {
        return STATUS_SUCCESS;
    }

    create.parent = "/clkdev";
    create.name = "isa-aux";
    create.type = NT_OB_TIMER;
    status = obCreateObject(&create, &clkdev);

    if (status != 0) {
        return status;
    }

    clkdev->data = &timer;
    isInit = true;
    return status;
}

static KTIMER timer = {
    .setCount = ktimerSetCount,
    .getCount = ktimerGetCount
};
