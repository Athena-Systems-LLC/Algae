/*
 * ALGAE executive process definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _EX_PROCESS_H_
#define _EX_PROCESS_H_ 1

#include <ke/types.h>
#include <ke/defs.h>
#include <md/pcb.h>     /* shared */
#include <rtl/queue.h>
#include <ntstatus.h>

#define PROCESS_UNAMED "unamed"
#define PROCESS_NAMELEN 32

/* exCreateProcess() flags */
#define PROCESS_KERNEL BIT(0)   /* Kernel thread */

typedef ULONG PID;

/*
 * Represents a process running on the
 * system
 *
 * @pid: Process ID to identify process
 * @name: Name of the process
 * @pcb: Process control block
 * @link: Scheduler queue link
 */
typedef struct process {
    PID pid;
    CHAR name[PROCESS_NAMELEN];
    PCB pcb;
    TAILQ_ENTRY(process) link;
} PROCESS;

/*
 * Create a process
 *
 * @name: Name of process
 * @result: Result is written here
 * @flags: Optional flags
 */
NTSTATUS exCreateProcess(const CHAR *name, PROCESS **result, USHORT flags);

#endif  /* !_EX_PROCESS_H_ */
