/*
 * ALGAE process scheduler
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/sched.h>
#include <ke/spinlock.h>
#include <ke/bugCheck.h>
#include <ke/timer.h>
#include <ke/kpcr.h>
#include <hal/mdefs.h>
#include <ex/process.h>
#include <ntstatus.h>

CACHELINE_ALIGNED static KPARB coreArbiter;
KTIMER *g_schedTimer;

/*
 * Get the next queue for load balancing based on
 * the arbiter mode
 */
static SCHED_QUEUE *
schedulerGetQueue(void)
{
    KPCR *kpcr;

    kpcr = keParbCycle(&coreArbiter);
    if (kpcr == NULL) {
        return NULL;
    }

    return &kpcr->queue;
}

NTSTATUS
keScheduleProc(SCHED_QUEUE *queue, PROCESS *process)
{
    if (process == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (queue == NULL) {
        queue = schedulerGetQueue();
    }

    /* This should not fail */
    if (queue == NULL) {
        keBugCheck("failed to acquire next scheduler queue\n");
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

    /* Is it empty? */
    if (queue->q.nelem == 0) {
        return STATUS_PROC_NOTFOUND;
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

void
kiSchedInit(void)
{
    NTSTATUS status;

    status = keTimerGetDescriptor("/clkdev/sched", &g_schedTimer);
    if (status != STATUS_SUCCESS) {
        keBugCheck("failed to get scheduler timer\n");
    }

    status = keParbReset(KPARB_ROUND_ROBIN, &coreArbiter);
    if (status != STATUS_SUCCESS) {
        keBugCheck("failed to reset scheduler core arbiter\n");
    }
}
