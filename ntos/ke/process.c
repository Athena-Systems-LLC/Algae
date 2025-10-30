/*
 * ALGAE process management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/process.h>
#include <hal/process.h>

NTSTATUS
keInitProcess(PROCESS *process, USHORT flags)
{
    if (process == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    return halInitPcb(&process->pcb, flags);
}
