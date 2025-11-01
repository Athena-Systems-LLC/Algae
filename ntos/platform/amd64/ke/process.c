/*
 * ALGAE process management HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/process.h>
#include <hal/mmu.h>
#include <md/lapic.h>
#include <md/frame.h>
#include <md/gdt.h>
#include <ke/timer.h>
#include <ke/bugCheck.h>
#include <ke/kpcr.h>
#include <ke/defs.h>
#include <mm/phys.h>
#include <rtl/string.h>

/*
 * The stack starts here and grows down
 */
#define STACK_TOP   0xBFFFFFFF
#define STACK_LEN   4096

extern KTIMER *g_schedTimer;

void
halProcKick(PCB *pcb)
{
    struct trapFrame *tf;

    tf = &pcb->tf;
    mmuWriteVas(&pcb->vas, 0);

    ASMV(
        "mov %0, %%rax\n"
        "mov %1, %%rbp\n"
        "push %2\n"
        "push %3\n"
        "push %4\n"
        "push %%rax\n"
        "push %5\n"
        "lfence\n"
        "swapgs\n"
        "iretq"
        :
        : "r" (tf->cs),
          "r" (tf->rbp),
          "i" (USER_DS | 3),
          "r" (tf->rsp),
          "m" (tf->rflags),
          "r" (tf->rip)
    );

    __builtin_unreachable();
}

NTSTATUS
halInitPcb(PCB *pcb, USHORT flags)
{
    struct trapFrame *tf;
    MM_REGION region;
    UCHAR ds, cs;
    void *stackBase;

    if (pcb == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    tf = &pcb->tf;
    ds = ISSET(flags, PROCESS_KERNEL) ? KERNEL_DS : USER_DS | 3;
    cs = ISSET(flags, PROCESS_KERNEL) ? KERNEL_CS : USER_CS | 3;
    pcb->stackPhysBase = mmAllocFrame(1);

    if (pcb->stackPhysBase == 0) {
        return STATUS_NO_MEMORY;
    }

    /* Initialize the trapframe */
    rtlMemset(&pcb->tf, 0, sizeof(pcb->tf));
    tf->cs = cs;
    tf->ss = ds;
    tf->rflags = 0x202;
    tf->rsp = STACK_TOP;
    mmuCreateVas(&pcb->vas);

    /* Map the stack */
    region.pBase = pcb->stackPhysBase;
    region.vBase = (void *)STACK_TOP;
    region.byteCount = PAGESIZE;
    stackBase = mmMapPages(
        &pcb->vas,
        &region,
        PAGE_USER_READWRITE,
        0
    );

    if (stackBase == NULL) {
        mmFreeFrame(pcb->stackPhysBase, 1);
        return STATUS_NO_MEMORY;
    }

    return STATUS_SUCCESS;
}

void
halProcSetIp(PCB *pcb, ULONG_PTR ip)
{
    struct trapFrame *tf;

    if (pcb == NULL) {
        return;
    }

    tf = &pcb->tf;
    tf->rip = ip;
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
    rtlMemcpy(frame, &pcb->tf, sizeof(*frame));
    core->curProc = next;

    /* Switch the address space */
    mmuWriteVas(&pcb->vas, 0);
done:
    kiLapicEoi();
    keTimerOneshotUsec(g_schedTimer, SCHED_QUANTUM);
}

void
halSchedEnter(void)
{
    keTimerOneshotUsec(g_schedTimer, SCHED_SHORT_QUANTUM);
    for (;;) {
        ASMV("sti; hlt");
    }
}
