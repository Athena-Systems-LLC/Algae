/*
 * ALGAE Internal boot up routines
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/boot.h>
#include <hal/uart.h>
#include <hal/intr.h>
#include <hal/cpu.h>
#include <acpi/acpi.h>
#include <mm/phys.h>

int
kiPlatformInit(void)
{
    halUartInit();
    mmPhysInit();
    acpiInit();
    halInitIrqChip();
    halInitSyscall();
    return 0;
}

int
kiProcessorInit(KPCR *kpcr)
{
    halCpuInit(kpcr);
    return 0;
}
