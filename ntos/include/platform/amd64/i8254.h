/*
 * ALGAE Intel i8254 driver
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_I8254_H_
#define _MACHINE_I8254_H_ 1

#include <ke/types.h>

#define I8254_COMMAND     0x43
#define I8254_CHANNEL_0   0x40
#define I8254_CHANNEL_2   0x42
#define I8254_DIVIDEND    1193182ULL

/*
 * Get the current counter value from the
 * timer
 */
USHORT pitGetCount(void);

/*
 * Set the counter value for the timer
 */
void pitSetCount(USHORT count);

/*
 * Set the timer reload to a desired frequency
 * in Hz
 */
void pitSetFrequency(UQUAD frequencyHz);

#endif  /* !_MACHINE_I8254_H_ */
