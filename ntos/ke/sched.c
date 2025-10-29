/*
 * ALGAE process scheduler
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/sched.h>
#include <ke/spinlock.h>
#include <hal/mdefs.h>
#include <ex/process.h>
#include <ntstatus.h>

NTSTATUS
keScheduleProc(SCHED_QUEUE *queue, PROCESS *process)
{
    if (queue == NULL || process == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    keAcquireSpinLock(&queue->lock);
    TAILQ_INSERT_TAIL(&queue->q, process, link);
    keReleaseSpinLock(&queue->lock);
    return STATUS_SUCCESS;
}

NTSTATUS
keDequeueProc(SCHED_QUEUE *queue, PROCESS **result)
{
    PROCESS *process;

    if (queue == NULL || process == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    keAcquireSpinLock(&queue->lock);
    process = TAILQ_FIRST(&queue->q);
    if (process == NULL) {
        keReleaseSpinLock(&queue->lock);
        return STATUS_PROC_NOTFOUND;
    }

    TAILQ_REMOVE(&queue->q, process, link);
    keReleaseSpinLock(&queue->lock);
    *result = process;
    return STATUS_SUCCESS;
}
