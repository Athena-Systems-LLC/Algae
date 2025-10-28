/*
 * ALGAE interrupt request level management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/irql.h>
#include <ke/assert.h>
#include <ke/defs.h>
#include <ke/kpcr.h>

/*
 * Reads the contents of the task priority register
 * which is windowed (no pun intended) by the CR8
 * register
 */
ALWAYS_INLINE static inline ULONG
readCr8(void)
{
    UQUAD cr8;

    ASMV(
        "mov %%cr8, %0"
        : "=r" (cr8)
        :
        : "memory"
    );

    return cr8;
}

ALWAYS_INLINE static inline void
writeCr8(ULONG value)
{
    UQUAD value64 = (UQUAD)value;

    ASMV(
        "mov %0, %%cr8"
        :
        : "r" (value64)
        : "memory"
    );
}

ULONG
halGetIrql(void)
{
    ULONG cr8;

    cr8 = readCr8();
    return cr8 & 0xF;
}

void
halSetIrql(ULONG irql)
{
    ULONG cr8;

    cr8 = readCr8();
    cr8 &= ~0xF;
    cr8 |= irql & 0xF;
    writeCr8(cr8);
}
