/*
 * ALGAE Kernel spinlocks
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_SPINLOCK_H_
#define _KE_SPINLOCK_H_ 1

#include <ke/types.h>

typedef struct {
    volatile BOOLEAN lock;
} KSPIN_LOCK;

/*
 * Hold a spinlock until released
 */
void keAcquireSpinLock(KSPIN_LOCK *lock);

/*
 * Release a spinlock
 */
void keReleaseSpinLock(KSPIN_LOCK *lock);

#endif
