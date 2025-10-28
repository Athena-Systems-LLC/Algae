/*
 * ALGAE interrupt request level management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/irql.h>
#include <ke/types.h>
#include <ke/bugCheck.h>
#include <hal/irql.h>

/*
 * Verifies if an IRQL is valid in the system
 *
 * XXX: All new IRQLs should be added to this list
 */
static BOOLEAN
isIrqlValid(UCHAR irql)
{
    switch (irql) {
    case PASSIVE_LEVEL:
    case DISPATCH_LEVEL:
    case CLOCK_LEVEL:
    case IPI_LEVEL:
    case POWER_LEVEL:
    case HIGH_LEVEL:
        return true;
    }

    return false;
}

ULONG
keGetIrql(void)
{
    return halGetIrql();
}

UCHAR
keIrqlRaise(UCHAR irql)
{
    ULONG curIrql;

    if (!isIrqlValid(irql)) {
        keBugCheck("got bad IRQL %d\n", irql);
    }

    /*
     * It must be higher than the current, otherwise the
     * driver is doing something weird.
     */
    curIrql = keGetIrql();
    if (irql < curIrql) {
        keBugCheck("IRQL less than current\n");
    }

    halSetIrql(irql);
    return curIrql;
}

UCHAR
keIrqlLower(UCHAR irql)
{
    ULONG curIrql;

    if (!isIrqlValid(irql)) {
        keBugCheck("got bad IRQL %d\n", irql);
    }

    /* Must be lower */
    curIrql = keGetIrql();
    if (irql > curIrql) {
        keBugCheck("IRQL greater than current\n");
    }

    halSetIrql(irql);
    return curIrql;
}
