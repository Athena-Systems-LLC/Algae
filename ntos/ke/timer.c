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

NTSTATUS
keTimerGetDescriptor(const CHAR *path, KTIMER **result)
{
    NT_OBJECT *obj;
    NTSTATUS status;

    if (path == NULL || result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    status = obLookupObject(path, &obj, 0);
    if (status != 0) {
        return STATUS_OBJECT_NOTFOUND;
    }

    if (obj->type != NT_OB_TIMER) {
        return STATUS_BAD_OBJ_TYPE;
    }

    *result = obj->data;
    return STATUS_SUCCESS;
}

NTSTATUS
keTimerGetCount(KTIMER *timer, USIZE *result)
{
    NTSTATUS status;
    USIZE count;

    if (timer == NULL || result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (timer->getCount == NULL) {
        return STATUS_NOT_SUPPORTED;
    }

    count = timer->getCount(timer);
    *result = count;
    return STATUS_SUCCESS;
}

NTSTATUS
keTimerSetCount(KTIMER *timer, USIZE count)
{
    NTSTATUS status;

    if (timer == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (timer->setCount == NULL) {
        return STATUS_NOT_SUPPORTED;
    }

    timer->setCount(timer, count);
    return STATUS_SUCCESS;
}

NTSTATUS
keTimerMsleep(KTIMER *timer, USIZE n)
{
    if (timer == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (timer->msleep == NULL) {
        return STATUS_NOT_SUPPORTED;
    }

    timer->msleep(timer, n);
    return STATUS_SUCCESS;
}

NTSTATUS
keTimerOneshotUsec(KTIMER *timer, USIZE n)
{
    if (timer == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (timer->oneshotUsec == NULL) {
        return STATUS_NOT_SUPPORTED;
    }

    timer->oneshotUsec(timer, n);
    return STATUS_SUCCESS;
}

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
    if (status != 0) {
        return status;
    }

    status = obAllocateDir(clkdevDir);
    if (status != STATUS_SUCCESS) {
        obFreeObject(clkdevDir);
        return status;
    }

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
