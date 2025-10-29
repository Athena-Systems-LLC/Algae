/*
 * ALGAE executive process definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _EX_PROCESS_H_
#define _EX_PROCESS_H_ 1

#include <ke/types.h>
#include <md/pcb.h>     /* shared */
#include <ntstatus.h>

#define PROCESS_UNAMED "unamed"
#define PROCESS_NAMELEN 32

typedef ULONG PID;

/*
 * Represents a process running on the
 * system
 *
 * @pid: Process ID to identify process
 * @name: Name of the process
 * @pcb: Process control block
 */
typedef struct {
    PID pid;
    CHAR name[PROCESS_NAMELEN];
    PCB pcb;
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
