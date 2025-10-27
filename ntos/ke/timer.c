/*
 * ALGAE kernel timer management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/timer.h>
#include <ke/types.h>
#include <ke/bugCheck.h>
#include <hal/timer.h>
#include <ob/object.h>
#include <ntstatus.h>

static BOOLEAN isInit = false;
static NT_OBJECT *clkdevDir = NULL;

/*
 * Create an object directory for timers to
 * be stored, this is called once during startup
 */
static NTSTATUS
timerCreateStore(void)
{
    NT_OBJECT_CREATE create;
    NTSTATUS status;

    /* Create a clock device directory */
    create.parent = "/";
    create.name = "clkdev";
    create.type = NT_OB_DIRECTORY;
    status = obCreateObject(&create, &clkdevDir);
    return status;
}

NTSTATUS
kiTimersInit(void)
{
    NTSTATUS status;

    /* Already initialized? */
    if (isInit) {
        return STATUS_SUCCESS;
    }

    /* Create the timer directory */
    status = timerCreateStore();
    if (status != STATUS_SUCCESS) {
        keBugCheck("kiTimersInit: failed to create clkdev dir\n");
    }

    /* Initialize the timers */
    status = halInitTimers();
    if (status != STATUS_SUCCESS) {
        keBugCheck("kiTimersInit: failed to initialize timers\n");
    }

    isInit = true;
    return STATUS_SUCCESS;
}
