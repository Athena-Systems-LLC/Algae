/*
 * ALGAE Kernel spinlocks
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/spinlock.h>
#include <md/mcb.h>         /* shared */

void
keAcquireSpinLock(KSPIN_LOCK *lock)
{
    while (__atomic_test_and_set(&lock->lock, __ATOMIC_ACQUIRE)) {
        MD_SPINWAIT();
    }
}

void
keReleaseSpinLock(KSPIN_LOCK *lock)
{
    __atomic_clear(&lock->lock, __ATOMIC_RELEASE);
}
