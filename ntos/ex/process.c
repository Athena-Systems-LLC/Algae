/*
 * ALGAE executive process management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/defs.h>
#include <ke/process.h>
#include <ex/process.h>
#include <ex/pool.h>
#include <rtl/string.h>
#include <ntstatus.h>

static volatile USIZE nextPid = 1;

NTSTATUS
exCreateProcess(const CHAR *name, PROCESS **result, USHORT flags)
{
    NTSTATUS status;
    PROCESS *process;
    USIZE nameLen;

    if (name == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    nameLen = rtlStrlen(name);

    /* Try to allocate memory for the process */
    process = exAllocatePool(POOL_NON_PAGED, sizeof(PROCESS));
    if (process == NULL) {
        return STATUS_NO_MEMORY;
    }

    /* Copy the name and truncate if needed */
    rtlMemcpy(
        process->name,
        name,
        MIN(PROCESS_NAMELEN, nameLen)
    );

    process->pid = nextPid++;
    status = keInitProcess(process);

    /* Did initializing succeed? */
    if (status != STATUS_SUCCESS) {
        exFreePool(process);
        return status;
    }

    *result = process;
    return status;
}
