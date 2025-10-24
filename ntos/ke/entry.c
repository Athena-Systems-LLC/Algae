/*
 * ALGAE Kernel entry.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/boot.h>
#include <ex/trace.h>

void
kMain(void)
{
    kiPlatformInit();
    kiProcessorInit();
    exTraceLn(EX_TRACE_INFO, "booting ALGAE...\n");
    for (;;);
}
