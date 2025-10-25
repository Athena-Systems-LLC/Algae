/*
 * ALGAE Memory Management Unit interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/mmu.h>
#include <mm/phys.h>
#include <mm/vm.h>
#include <ke/bugCheck.h>
#include <ke/defs.h>

/*
 * Page-Table Entry (PTE) flags
 *
 * See Intel SDM Vol 3A, Section 4.5, Table 4-19
 */
#define PTE_ADDR_MASK   0x000FFFFFFFFFF000
#define PTE_P           BIT(0)        /* Present */
#define PTE_RW          BIT(1)        /* Writable */
#define PTE_US          BIT(2)        /* User r/w allowed */
#define PTE_PWT         BIT(3)        /* Page-level write-through */
#define PTE_PCD         BIT(4)        /* Page-level cache disable */
#define PTE_ACC         BIT(5)        /* Accessed */
#define PTE_DIRTY       BIT(6)        /* Dirty (written-to page) */
#define PTE_PS          BIT(7)        /* Page size */
#define PTE_GLOBAL      BIT(8)        /* Global / sticky map */
#define PTE_NX          BIT(63)       /* Execute-disable */

typedef enum {
    MAP_LEVEL_PT,
    MAP_LEVEL_PD,
    MAP_LEVEL_PDPT,
    MAP_LEVEL_PML4,
    _MAP_LEVEL_MAX
} MAP_LEVEL;

/*
 * Flush an entry from the TLB
 */
static inline void
mmuTlbFlush(ULONG_PTR vaddr)
{
    ASMV(
        "invlpg (%0)"
        :
        : "r" (vaddr)
        : "memory"
    );
}

/*
 * Get the index of a table label using a virtual
 * address as a key
 */
static inline USHORT
mmuGetLevelIdx(ULONG_PTR virtAddr, MAP_LEVEL level)
{
    switch (level) {
    case MAP_LEVEL_PT:
        return (virtAddr >> 12) & 0x1FF;
    case MAP_LEVEL_PD:
        return (virtAddr >> 21) & 0x1FF;
    case MAP_LEVEL_PDPT:
        return (virtAddr >> 30) & 0x1FF;
    case MAP_LEVEL_PML4:
        return (virtAddr >> 39) & 0x1FF;
    default:
        keBugCheck("bad level in %s()\n", __func__);
    }
}

/*
 * Get a table entry at a specific level via iterative
 * descent, returns the physical address
 *
 * @vas: Current virtual address space
 * @virtAddr: Virtual address to use as key
 * @level: Desired level index
 * @alloc: Allocate tables if needed
 *
 * XXX: Returned values of zero indicate failure
 */
static ULONG_PTR
mmuGetLevelPhys(MMU_VAS *vas, ULONG_PTR virtAddr, MAP_LEVEL level, BOOLEAN alloc)
{
    MAP_LEVEL curLevel = MAP_LEVEL_PML4;
    ULONG_PTR *curTable;
    ULONG_PTR tmpVal;
    USHORT levelIdx;

    if (vas == NULL) {
        return 0;
    }

    if (level >= _MAP_LEVEL_MAX) {
        return 0;
    }

    /* PML4 */
    curTable = PHYS_TO_VIRT(vas->cr3);

    /* Begin the iterative descent */
    while (curLevel > level) {
        levelIdx = mmuGetLevelIdx(virtAddr, curLevel);
        tmpVal = curTable[levelIdx];

        /* Is this a present entry? */
        if (ISSET(tmpVal, PTE_P)) {
            tmpVal = tmpVal & PTE_ADDR_MASK;
            curTable = PHYS_TO_VIRT(tmpVal);
        }

        /* Don't continue if we can't alloc */
        if (!alloc) {
            return 0;
        }

        tmpVal = mmAllocFrame(1);
        if (tmpVal == 0) {
            return 0;
        }

        curTable[levelIdx] = tmpVal;
        curTable[levelIdx] |= (PTE_P | PTE_RW | PTE_US);
        curTable = PHYS_TO_VIRT(tmpVal);
        --curLevel;
    }

    return VIRT_TO_PHYS(curTable);
}

/*
 * Converts PAGE_* types to PTE flags
 */
static QUAD
mmuGetTableFlags(USHORT mapType)
{
    QUAD pteFlags = PTE_P | PTE_NX;

    switch (mapType) {
    case PAGE_READONLY:
        return pteFlags;
    case PAGE_READWRITE:
        pteFlags |= PTE_RW;
        return pteFlags;
    case PAGE_EXECUTE_READ:
        pteFlags &= ~PTE_NX;
        return pteFlags;
    }

    return pteFlags;
}

int
mmuWriteVas(MMU_VAS *vas, USHORT flags)
{
    ASMV(
        "mov %0, %%cr3"
        :
        : "r" (vas->cr3)
        : "memory"
    );

    return 0;
}

int
mmuReadVas(MMU_VAS *res)
{
    ASMV(
        "mov %%cr3, %0"
        : "=r" (res->cr3)
        :
        : "memory"
    );

    return 0;
}

int
mmuMapSingle(MMU_VAS *vas, ULONG_PTR vBase, ULONG_PTR pBase, USHORT mFlags)
{
    USHORT pteIdx;
    ULONG_PTR ptePhys;
    ULONG_PTR *pte;
    QUAD mapFlags;

    if (vas == NULL) {
        return -1;
    }

    /* Align bases down and grab the page table */
    vBase = ALIGN_DOWN(vBase, PAGESIZE);
    pBase = ALIGN_DOWN(pBase, PAGESIZE);
    ptePhys = mmuGetLevelPhys(
        vas,
        vBase,
        MAP_LEVEL_PT,
        true
    );

    if (ptePhys == 0) {
        return -1;
    }

    pte = PHYS_TO_VIRT(ptePhys);
    pteIdx = mmuGetLevelIdx(vBase, MAP_LEVEL_PT);
    mapFlags = mmuGetTableFlags(mFlags);

    /*
     * Update the entry and flush it from the TLB
     * so we don't get any stale cache hits
     */
    pte[pteIdx] = (pBase | mapFlags);
    mmuTlbFlush(vBase);
    return 0;
}
