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
    entry->rsp0Lo = rsp0 & 0xFFFFFFFF;
    entry->rsp0Hi = (rsp0 >> 32) & 0xFFFFFFFF;
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
