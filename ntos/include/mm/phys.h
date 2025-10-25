/*
 * ALGAE Physical Memory Management interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MM_PHYS_H_
#define _MM_PHYS_H_ 1

#include <ke/types.h>

/*
 * Allocate `count' frames and return the
 * physical base to the allocated frames
 */
ULONG_PTR mmAllocFrame(USIZE count);

/*
 * Free `count' frames at `base'
 */
void mmFreeFrame(ULONG_PTR base, USIZE count);

/*
 * Initialize the physical memory manager
 */
void mmPhysInit(void);

#endif  /* !_MM_PHYS_H_ */
