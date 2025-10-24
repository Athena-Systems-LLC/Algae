/*
 * ALGAE port I/O interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_PIO_H_
#define _HAL_PIO_H_ 1

#include <ke/types.h>

/*
 * I/O bus input routines
 */
UCHAR halBusInb(USHORT port);
USHORT halBusInw(USHORT port);
ULONG halBusInl(USHORT port);

/*
 * I/O bus output routines
 */
void halBusOutb(USHORT port, UCHAR val);
void halBusOutw(USHORT port, USHORT val);
void halBusOutl(USHORT port, ULONG val);

#endif  /* !_HAL_PIO_H_ */
