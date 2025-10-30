/*
 * ALGAE process management HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/process.h>
#include <hal/mmu.h>
#include <ke/timer.h>
#include <ke/defs.h>
#include <ex/trace.h>

extern KTIMER *g_schedTimer;

NTSTATUS
halInitPcb(PCB *pcb)
{
    if (pcb == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    mmuReadVas(&pcb->vas);
    return STATUS_SUCCESS;
}

NTSTATUS
halSchedSwitch(struct trapFrame *frame)
{
    for (;;) {
        ASMV("cli; hlt");
    }
}

void
halSchedEnter(void)
{
    keTimerOneshotUsec(g_schedTimer, 5000);
    for (;;);
}
