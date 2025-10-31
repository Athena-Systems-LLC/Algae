/*
 * ALGAE Memory Management Unit interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_MMU_H_
#define _HAL_MMU_H_

#include <ke/types.h>
#include <mm/vm.h>
#include <md/vas.h>     /* standard */
#include <ntstatus.h>

#define PAGE_READONLY       0x00
#define PAGE_READWRITE      0x01
#define PAGE_EXECUTE_READ   0x02

/*
 * Map a single page of memory
 *
 * @vas: Virtual address space to map in
 * @vBase: Virtual base address to map
 * @pBase: Physical base address to map
 * @mFlags: Map flags
 *
 * XXX: All base addresses are to be aligned down on the machine
 *      page boundary
 *
 * Returns zero on success, otherwise a less than
 * zero value on failure.
 */
NTSTATUS mmuMapSingle(
    MMU_VAS *vas, ULONG_PTR vBase,
    ULONG_PTR pBase, USHORT mFlags
);

/*
 * Unmap a virtual address from the desired virtual
 * address space
 *
 * @vas: VAS to unmap virtual address from
 * @vBase: Virtual address base to unmap
 */
NTSTATUS mmuUnmapSingle(MMU_VAS *vas, ULONG_PTR vBase);

/*
 * Update the current virtual address space
 * for the processor
 *
 * @vas: VAS to update to
 * @flags: Optional flags
 *
 * Returns zero on success, otherwise a less than
 * zero value on failure.
 */
NTSTATUS mmuWriteVas(MMU_VAS *vas, USHORT flags);

/*
 * Read the current virtual address space
 *
 * @res: Result is written here
 *
 * Returns zero on success, otherwise a less than
 * zero value on failure.
 */
NTSTATUS mmuReadVas(MMU_VAS *res);

/*
 * Allocate a new virtual address space
 *
 * @res: Result is written here
 */
NTSTATUS mmuCreateVas(MMU_VAS *res);

#endif  /* !_HAL_MMU_H_ */
