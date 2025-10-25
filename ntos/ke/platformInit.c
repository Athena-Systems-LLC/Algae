/*
 * ALGAE Internal boot up routines
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/boot.h>
#include <hal/uart.h>
#include <hal/cpu.h>
#include <mm/phys.h>

int
kiPlatformInit(void)
{
    halUartInit();
    mmPhysInit();
    return 0;
}

int
kiProcessorInit(void)
{
    halCpuInit();
    return 0;
}
