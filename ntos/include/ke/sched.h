/*
 * ALGAE process scheduler
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_SCHED_H_
#define _KE_SCHED_H_ 1

#include <ke/types.h>
#include <ke/spinlock.h>
#include <rtl/queue.h>
#include <ex/process.h>
#include <ntstatus.h>

typedef struct {
    TAILQ_HEAD(, process) q;
    KSPIN_LOCK lock;
} SCHED_QUEUE;

/*
 * Mark a process as ready to run and enqueue it
 * into the scheduler queue
 *
 * @queue: Scheduler queue to put process in
 * @process: Process to enqueue
 */
NTSTATUS keScheduleProc(SCHED_QUEUE *queue, PROCESS *process);

#endif  /* !_KE_SCHED_H_ */
