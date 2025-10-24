/*
 * ALGAE Boot up parameters
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_BOOTPARAM_H_
#define _KE_BOOTPARAM_H_ 1

#include <ke/types.h>

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
};

/*
 * Get the list of boot parameters from the
 * loader
 *
 * @res: New bootparams is written here
 * @flags: Optional flags
 *
 * Returns zero on success
 */
int keGetBootParams(struct bootParams *res, int flags);

#endif  /* !_KE_BOOTPARAM_H_ */
