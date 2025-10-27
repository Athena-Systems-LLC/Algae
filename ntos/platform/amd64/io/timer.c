/*
 * ALGAE timer HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/timer.h>
#include <ntstatus.h>

/*
 * Initializes timers such as HPET, etc
 */
NTSTATUS
halInitTimers(void)
{
    return STATUS_SUCCESS;
}
