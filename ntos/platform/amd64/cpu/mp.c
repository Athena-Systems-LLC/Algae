/*
 * ALGAE multiprocessor support
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/defs.h>
#include <ex/trace.h>

SECTION(".trampoline")
UCHAR trampoline[4096];

void
kiMpInit(void)
{
    /* XXX: STUB */
}
