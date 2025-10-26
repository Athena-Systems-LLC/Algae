/*
 * ALGAE Interrupt chip initialization
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <md/ioapic.h>
#include <hal/intr.h>

int
halInitIrqChip(void)
{
    kiIoApicInit();
    return 0;
}
