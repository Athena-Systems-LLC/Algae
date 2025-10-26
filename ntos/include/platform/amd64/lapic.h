/*
 * ALGAE Local APIC interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_LAPIC_H_
#define _MACHINE_LAPIC_H_ 1

#include <ke/types.h>

/*
 * Internal routine used to initialize the
 * Local APIC on the current processor core
 */
void kiLapicInit(void);

#endif /* !_MACHINE_LAPIC_H_ */
