/*
 * ALGAE executive memory pool allocation
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _EX_POOL_H_
#define _EX_POOL_H 1

#include <ke/types.h>

typedef enum {
    POOL_NON_PAGED,
} POOL_TYPE;

/*
 * Internal function to initialize the pool
 * subsystem
 */
void kiPoolInit(void);

/*
 * Allocate a pool of memory of a specific type and
 * return the base, otherwise NULL on failure
 *
 * @type: Pool type (see POOL_*)
 * @length: Number of bytes to allocate
 */
void *exAllocatePool(POOL_TYPE type, USIZE length);

/*
 * Release a pool back to the operating system
 *
 * @pool: Pool of memory to free
 */
void exFreePool(void *pool);

#endif  /* !_EX_POOL_H */
