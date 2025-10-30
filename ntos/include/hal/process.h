/*
 * ALGAE process management HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_PROCESS_H_
#define _HAL_PROCESS_H_ 1

#include <md/pcb.h>     /* shared */
#include <md/frame.h>   /* shared */
#include <ntstatus.h>

/*
 * Initialize a process control block
 */
NTSTATUS halInitPcb(PCB *pcb);

/*
 * Switch the current thread
 */
void halSchedSwitch(struct trapFrame *frame);

/*
 * Enter the scheduler
 */
void halSchedEnter(void);

#endif  /* !_HAL_PROCESS_H_ */
