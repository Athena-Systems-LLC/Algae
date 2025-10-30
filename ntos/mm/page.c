/*
 * ALGAE virtual memory page management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <mm/vm.h>
#include <mm/phys.h>

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
