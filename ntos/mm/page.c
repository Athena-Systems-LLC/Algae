/*
 * ALGAE virtual memory page management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <mm/vm.h>
#include <mm/phys.h>
#include <ke/defs.h>
#include <hal/mmu.h>

void *
mmAllocPage(USIZE count, USHORT flags)
{
    ULONG_PTR pBase;

    pBase = mmAllocFrame(count);
    if (pBase == 0) {
        return NULL;
    }

    return PHYS_TO_VIRT(pBase);
}

void
mmFreePages(void *base, USIZE count)
{
    ULONG_PTR pBase;

    if (base == NULL) {
        return;
    }

    pBase = VIRT_TO_PHYS(pBase);
    mmFreeFrame(pBase, count);
}

void *
mmMapPages(MMU_VAS *vas, MM_REGION *region, USHORT prot, USHORT flags)
{
    NTSTATUS status;

    if (region == NULL) {
        return NULL;
    }

    /* Ensure everything is aligned correctly */
    region->byteCount = ALIGN_UP(region->byteCount, PAGESIZE);
    region->pBase = ALIGN_DOWN(region->pBase, PAGESIZE);
    region->vBase = (void *)ALIGN_DOWN((ULONG_PTR)region->vBase, PAGESIZE);

    /* Begin mapping everything */
    for (ULONG_PTR off = 0; off < region->byteCount; off += PAGESIZE) {
        status = mmuMapSingle(
            vas,
            (ULONG_PTR)region->vBase + off,
            region->pBase + off,
            prot
        );

        /*
         * TODO: We need to fix the big ass hole that we left in
         *       the address space, undo the fuck up that we fucked
         *       up or something like that...
         */
        if (status != STATUS_SUCCESS) {
            return NULL;
        }
    }
    return region->vBase;
}

MMU_VAS
mmGetCurrentVas(void)
{
    MMU_VAS vas;

    mmuReadVas(&vas);
    return vas;
}
