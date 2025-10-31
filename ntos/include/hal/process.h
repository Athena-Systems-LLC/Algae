/*
 * ALGAE process management HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_PROCESS_H_
#define _HAL_PROCESS_H_ 1

#include <ke/defs.h>
#include <md/pcb.h>     /* shared */
#include <md/frame.h>   /* shared */
#include <ntstatus.h>

/*
 * Initialize a process control block
 *
 * @pcb: PCB to initialize
 * @flags: Optional flags
 */
NTSTATUS halInitPcb(PCB *pcb, USHORT flags);

/*
 * Switch the current thread
 */
void halSchedSwitch(struct trapFrame *frame);

/*
 * Set the instruction pointer of a process to
 * a specific value
 */
void halProcSetIp(PCB *pcb, ULONG_PTR ip);

/*
 * Kick a process into userland
 */
NORETURN void halProcKick(PCB *pcb);

/*
 * Enter the scheduler
 */
void halSchedEnter(void);

#endif  /* !_HAL_PROCESS_H_ */
