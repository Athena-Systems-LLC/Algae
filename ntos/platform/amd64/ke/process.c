/*
 * ALGAE process management HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/process.h>
#include <hal/mmu.h>
#include <md/lapic.h>
#include <ke/timer.h>
#include <ke/bugCheck.h>
#include <ke/kpcr.h>
#include <ke/defs.h>
#include <rtl/string.h>

extern KTIMER *g_schedTimer;

NTSTATUS
halInitPcb(PCB *pcb, USHORT flags)
{
    if (pcb == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    mmuReadVas(&pcb->vas);
    rtlMemset(&pcb->tf, 0, sizeof(pcb->tf));
    return STATUS_SUCCESS;
}

void
halSchedSwitch(struct trapFrame *frame)
{
    KPCR *core = keGetCore();
    PCB *pcb = NULL;
    PROCESS *self, *next = NULL;
    NTSTATUS status;

    if (core == NULL) {
        return;
    }

    /*
     * Switch if we have a process currently
     * running
     */
    self = core->curProc;
    if (self != NULL) {
        status = keScheduleProc(&core->queue, self);
        if (status != STATUS_SUCCESS) {
            goto done;
        }

        /* Save the current trapframe */
        pcb = &self->pcb;
        rtlMemcpy(&pcb->tf, frame, sizeof(pcb->tf));
    }

    status = keDequeueProc(&core->queue, &next);
    if (status != STATUS_SUCCESS) {
        goto done;
    }

    /* Load the next frame into the live frame */
    pcb = &next->pcb;
    rtlMemcpy(frame, &pcb->tf, 1);
    core->curProc = next;

    /* Switch the address space */
    mmuWriteVas(&pcb->vas, 0);
done:
    kiLapicEoi();
    keTimerOneshotUsec(g_schedTimer, SCHED_SHORT_QUANTUM);
}

void
halSchedEnter(void)
{
    keTimerOneshotUsec(g_schedTimer, SCHED_SHORT_QUANTUM);
    for (;;) {
        ASMV("sti; hlt");
    }
}
