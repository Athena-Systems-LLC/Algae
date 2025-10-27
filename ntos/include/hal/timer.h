/*
 * ALGAE timer HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_ 1

#include <ke/types.h>
#include <ntstatus.h>

/*
 * Initialize timer subsystems, called once on
 * start up
 */
NTSTATUS halInitTimers(void);

#endif  /* !_HAL_TIMER_H_ */
