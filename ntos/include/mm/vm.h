/*
 * ALGAE Virtual Memory Management interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MM_VM_H_
#define _MM_VM_H_ 1

#include <ke/bootParam.h>
#include <ke/types.h>
#include <md/vas.h>     /* shared */

#define PAGESIZE 4096
#define PHYS_TO_VIRT(PA) (void *)((PA) + keGetKernelBase())
#define VIRT_TO_PHYS(VIRT) (((ULONG_PTR)(VIRT)) - keGetKernelBase())

/*
 * Represents a contigious virtual memory region
 *
 * @pBase: Physical base address
 * @vBase: Virtual base address
 * @byteCount: Number of bytes in region
 */
typedef struct {
    ULONG_PTR pBase;
    void *vBase;
    USIZE byteCount;
} MM_REGION;

/*
 * Allocate virtual memory pages
 *
 * @count: Number of pages to allocate
 * @flags: Optional flags
 *
 * Returns the base of the contiguous memory region
 * on success, otherwise NULL on failure
 */
void *mmAllocPage(USIZE count, USHORT flags);

/*
 * Free virtual memory pages
 *
 * @base: Address base to free at
 * @count: Number of pages to free
 */
void mmFreePages(void *base, USIZE count);

/*
 * Map a contigious region of pages
 *
 * @vas: Virtual address space to map in
 * @region: Region descriptor of area to map
 * @prot: Protection flags
 * @flags: Optional flags
 *
 * Returns the virtual base on success, otherwise a value
 * of NULL on failure
 */
void *mmMapPages(
    MMU_VAS *vas, MM_REGION *region,
    USHORT prot, USHORT flags
);

#endif  /* !_MM_VM_H_ */
