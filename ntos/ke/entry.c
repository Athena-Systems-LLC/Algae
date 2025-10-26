/*
 * ALGAE Kernel entry.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/boot.h>
#include <ex/pool.h>

void
kMain(void)
{
    kiPlatformInit();
    kiProcessorInit();
    kiBootScreen();
    kiPoolInit();

    for (;;);
}
