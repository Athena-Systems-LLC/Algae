/*
 * ALGAE Boot up parameters
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_BOOTPARAM_H_
#define _KE_BOOTPARAM_H_ 1

#include <ke/types.h>
#include <ntstatus.h>

/*
 * Framebuffer parameters
 *
 * @io: MMIO base
 */
struct fbParams {
    ULONG *io;
    ULONG width;
    ULONG height;
    ULONG pitch;
    UCHAR bpp;
};

/*
 * Boot parameters available
 */
struct bootParams {
    struct fbParams fbParams;
    void *acpiRsdp;
};

/*
 * Get the list of boot parameters from the
 * loader
 *
 * @res: New bootparams is written here
 * @flags: Optional flags
 */
NTSTATUS keGetBootParams(struct bootParams *res, int flags);

/*
 * Get the base address of the data of some
 * module
 *
 * @path: Path of module to fetch
 *
 * Returns the base of the module
 */
char *keGetBootModule(const char *path);

/*
 * Return the kernel base offset which is used to convert
 * certain lower half physical addresses to higher half
 * physical addresses.
 */
ULONG_PTR keGetKernelBase(void);

#endif  /* !_KE_BOOTPARAM_H_ */
