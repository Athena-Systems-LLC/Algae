/*
 * ALGAE timer HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/timer.h>
#include <md/i8254.h>
#include <md/io/hpet.h>
#include <ex/trace.h>
#include <ob/object.h>
#include <ntstatus.h>

/*
 * Initializes timers such as HPET, etc
 */
NTSTATUS
halInitTimers(void)
{
    NTSTATUS status;

    /*
     * Try to initialize the legacy i8254, if it fails we
     * can probably be fine without it, just log the error.
     */
    status = pitInit();
    if (status != STATUS_SUCCESS) {
        exTrace(EX_TRACE_ERR, "halInitTimers: failed to register i8254\n");
    }

    kiHpetInit();
    return STATUS_SUCCESS;
}
