/*
 * ALGAE Kernel Processor Arbitration
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/kpcr.h>
#include <rtl/string.h>

NTSTATUS
keParbReset(KPARB_METHOD mode, KPARB *result)
{
    if (result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    rtlMemset(result, 0, sizeof(KPARB));
    result->method = mode;
    return STATUS_SUCCESS;
}

KPCR *
keParbCycle(KPARB *arbiter)
{
    KPCR *retval = NULL;

    keAcquireSpinLock(&arbiter->lock);
    switch (arbiter->method) {
    case KPARB_ROUND_ROBIN:
        retval = keCpuGet(arbiter->semaphore);
        ++arbiter->semaphore;

        /* Wrap if needed */
        if (arbiter->semaphore >= keCpuCount()) {
            arbiter->semaphore = 0;
        }

        break;
    }

    keReleaseSpinLock(&arbiter->lock);
    return retval;
}
