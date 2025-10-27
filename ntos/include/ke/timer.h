/*
 * ALGAE kernel timer management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_TIMER_H_
#define _KE_TIMER_H_ 1

#include <ke/types.h>
#include <ntstatus.h>

/*
 * Represents a kernel timer used for things such as
 * sleeping and time keeping
 */
typedef struct ktimer {
    NTSTATUS(*msleep)(struct ktimer *self, USIZE msec);
    NTSTATUS(*usleep)(struct ktimer *self, USIZE usec);
} KTIMER;

/*
 * Internal routine to initialize system timers
 */
NTSTATUS kiTimersInit(void);

 #endif /* !_KE_TIMER_H_ */