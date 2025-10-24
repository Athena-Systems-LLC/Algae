/*
 * ALGAE MD CPU initialization
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/cpu.h>
#include <ke/types.h>
#include <md/idt.h>
#include <md/gdt.h>

static struct gdtr gdtr;

void
halCpuInit(void)
{
    gdtr.offset = (ULONG_PTR)&g_gdtData[0];
    gdtr.limit = sizeof(g_gdtData) - 1;
    kiGdtLoad(&gdtr);
}
