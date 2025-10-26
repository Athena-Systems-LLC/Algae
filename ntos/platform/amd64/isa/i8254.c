/*
 * ALGAE Intel i8254 driver
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <hal/pio.h>
#include <md/i8254.h>

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
