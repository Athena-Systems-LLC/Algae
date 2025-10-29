/*
 * ALGAE Machine Defs
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_MDEFS_H_
#define _HAL_MDEFS_H_

#include <md/mcb.h>     /* shared */

#ifndef CACHELINE_SIZE
#define CACHELINE_SIZE 64
#endif  /* !CACHELINE_SIZE */

#define CACHELINE_ALIGNED                   \
    __attribute__((                         \
        __aligned__(CACHELINE_SIZE),        \
        __section__(".cacheline_aligned")   \
    ))

#endif  /* !_HAL_MDEFS_H_ */
