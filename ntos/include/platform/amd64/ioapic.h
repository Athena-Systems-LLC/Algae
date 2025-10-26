/*
 * ALGAE I/O APIC interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_IOAPIC_H_
#define _MACHINE_IOAPIC_H_ 1

#include <ke/types.h>

/*
 * Initialize the I/O APIC chip
 */
void kiIoApicInit(void);

/*
 * Set the mask for the desired Global System Interrupt
 * number
 *
 * @gsi: Global System Interrupt
 * @mask: 1=masked, 0=unmasked
 */
void kiIoApicMaskGsi(UCHAR gsi, BOOLEAN mask);

#endif  /* !_MACHINE_IOAPIC_H_ */
