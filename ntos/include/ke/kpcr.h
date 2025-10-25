/*
 * ALGAE Kernel Processor Control Region
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_KPCR_H_
#define _HAL_KPCR_H_ 1

#include <ke/types.h>
#include <md/mcb.h>     /* standard */

/*
 * The kernel processor control region is standard between
 * architectures and contains general MI information about
 * a processor core
 *
 * @vId: Virtual / logical ID (assigned by the kernel)
 * @core: MD information
 */
typedef struct kpcr {
    USHORT vId;
    MCB core;
    struct kpcr *self;
} KPCR;

/*
 * Get the KPCR for the currently executing
 * processor core
 *
 * XXX: This counts as HAL and also counts as KE as
 *      its a really general function.
 */
KPCR *keGetCore(void);

#endif  /* !_HAL_KPCR_H_ */
