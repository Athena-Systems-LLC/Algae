/*
 * ALGAE interrupt request level management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_IPI_H_
#define _HAL_IPI_H_ 1

#include <ke/types.h>
#include <ke/kpcr.h>

/* Interrupt request levels (IRQLs) */
#define PASSIVE_LEVEL   0x1
#define DISPATCH_LEVEL  0x2
#define CLOCK_LEVEL     0x3
#define IPI_LEVEL       0xD
#define POWER_LEVEL     0xE
#define HIGH_LEVEL      0xF

/*
 * Get the current interrupt request level
 *
 * @core: Processor core to get the IRQL from
 */
ULONG halGetIrql(void);

/*
 *
 * @core: Processor core to set the IRQL for
 * @irql: IRQL to set
 */
void halSetIrql(ULONG irql);

#endif  /* !_HAL_IPI_H_ */
