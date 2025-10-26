/*
 * ALGAE executive memory pool allocation
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ex/pool.h>
#include <ke/types.h>
#include <ke/bugCheck.h>
#include <mm/phys.h>
#include <mm/tlsf.h>
#include <mm/vm.h>

#define POOLSIZE_BYTES 0x2000000  /* 4 MiB */
#define POOLSIZE_PAGES (POOLSIZE_BYTES / PAGESIZE)

static BOOLEAN isInit = false;
static ULONG_PTR poolPhysBase;
static tlsf_t tlsfCtx;
static void *poolVirtBase;

void
exFreePool(void *pool)
{
    tlsf_free(tlsfCtx, pool);
}

void *
exAllocatePool(POOL_TYPE type, USIZE length)
{
    if (!isInit) {
        return NULL;
    }

    return tlsf_malloc(tlsfCtx, length);
}

void
kiPoolInit(void)
{
    if (isInit) {
        return;
    }

    poolPhysBase = mmAllocFrame(POOLSIZE_PAGES);
    if (poolPhysBase == 0) {
        keBugCheck("could not initialize root pool\n");
    }

    poolVirtBase = PHYS_TO_VIRT(poolPhysBase);
    tlsfCtx = tlsf_create_with_pool(poolVirtBase, POOLSIZE_BYTES);
    isInit = true;
}
