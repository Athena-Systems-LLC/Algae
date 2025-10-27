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
    USIZE(*getCount)(struct ktimer *self);
    void(*setCount)(struct ktimer *self, USIZE count);
} KTIMER;

/*
 * Get a pointer to a timer descriptor
 *
 * @path: Path of timer descriptor to get
 * @result: Timer pointer written here
 */
NTSTATUS keTimerGetDescriptor(const CHAR *path, KTIMER **result);

/*
 * Get a timer's counter value
 *
 * @timer: Timer get count from
 * @result: Result is written here
 */
NTSTATUS keTimerGetCount(KTIMER *timer, USIZE *result);

/*
 * Set a timer's counter value
 *
 * @timer: Timer to set count for
 * @count: Count to write
 */
NTSTATUS keTimerSetCount(KTIMER *timer, USIZE count);

/*
 * Internal routine to initialize system timers
 */
NTSTATUS kiTimersInit(void);

 #endif /* !_KE_TIMER_H_ */