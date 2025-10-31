/*
 * ALGAE Task state segment
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <md/tss.h>
#include <md/tssVar.h>
#include <md/mcb.h>
#include <mm/phys.h>
#include <mm/vm.h>
#include <ex/pool.h>
#include <rtl/string.h>
#include <ke/kpcr.h>
#include <ke/assert.h>
#include <ke/bugCheck.h>

#define offsetof(st, m) ((USIZE)&(((st *)0)->m))
#define STACK_SIZE_PAGES 1

static void
allocKernelStack(TSS_ENTRY *entry)
{
    ULONG_PTR rsp0;

    rsp0 = mmAllocFrame(STACK_SIZE_PAGES);
    if (rsp0 == 0) {
        keBugCheck("failed to allocate TSS rsp0\n");
    }

    rsp0 += STACK_SIZE_PAGES * PAGESIZE;
    rsp0 += keGetKernelBase();
    entry->rsp0Lo = rsp0 & 0xFFFFFFFF;
    entry->rsp0Hi = (rsp0 >> 32) & 0xFFFFFFFF;
}

NTSTATUS
tssAllocateStack(TSS_STACK *result)
{
    if (result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    result->stackBase = mmAllocFrame(STACK_SIZE_PAGES);
    if (result->stackBase == 0) {
        return STATUS_NO_MEMORY;
    }

    result->stackBase = (ULONG_PTR)PHYS_TO_VIRT(result->stackBase);
    return STATUS_SUCCESS;
}

NTSTATUS
tssSetIntrStack(KPCR *kpcr, UCHAR istNumber, TSS_STACK *stack)
{
    MCB *mcb;
    TSS_ENTRY *tss;

    if (kpcr == NULL || stack == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    mcb = &kpcr->core;
    tss = &mcb->tss;

    switch (istNumber) {
    case 0:
        tss->ist1Lo = stack->stackLow;
        tss->ist1Hi = stack->stackHigh;
        break;
    case 1:
        tss->ist2Lo = stack->stackLow;
        tss->ist2Hi = stack->stackHigh;
        break;
    case 2:
        tss->ist3Lo = stack->stackLow;
        tss->ist3Hi = stack->stackHigh;
        break;
    case 3:
        tss->ist4Lo = stack->stackLow;
        tss->ist4Hi = stack->stackHigh;
        break;
    case 4:
        tss->ist5Lo = stack->stackLow;
        tss->ist5Hi = stack->stackHigh;
        break;
    case 5:
        tss->ist6Lo = stack->stackLow;
        tss->ist6Hi = stack->stackHigh;
        break;
    case 6:
        tss->ist7Lo = stack->stackLow;
        tss->ist7Hi = stack->stackHigh;
        break;
    default:
        return STATUS_INVALID_PARAM;
    }

    return STATUS_SUCCESS;
}

void
tssWrite(KPCR *kpcr, TSS_DESC *desc)
{
    TSS_ENTRY *tss;
    ULONG_PTR tssBase, tmp;
    MCB *mcb;

    mcb = &kpcr->core;
    tss = &mcb->tss;
    allocKernelStack(tss);
    tssBase = (ULONG_PTR)&mcb->tss;

    /* Set descriptor fields */
    desc->seglimit = sizeof(TSS_ENTRY) - 1;
    desc->p = 1;
    desc->g = 0;
    desc->avl = 0;
    desc->dpl = 0;
    desc->type = 0x9; /* TSS: 0x9 (0b1001) */

    /* Point it to the TSS */
    desc->baseLo16 = tssBase & 0xFFFF;
    desc->baseMid8 = (tssBase >> 16) & 0xFF;
    desc->baseHiMid8 = (tssBase >> 24) & 0xFF;
    desc->baseHi32 = (tssBase >> 32) & 0xFFFFFFFF;

    tss = &mcb->tss;
    rtlMemset(tss->iomap, 0xFF, sizeof(tss->iomap));
    tss->iobase = offsetof(TSS_ENTRY, iomap);
}
