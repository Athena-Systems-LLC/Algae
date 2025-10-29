/*
 * ALGAE CPU HAL definitions.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_CPU_H_
#define _HAL_CPU_H_ 1

#include <ke/kpcr.h>

typedef enum {
    CPU_HALT_SELF,  /* Halt current core */
} CPU_HALT_MODE;

void halCpuInit(KPCR *kpcr);

/*
 * Halt the CPU with a specific mode
 *
 * @mode: See CPU_HALT_MODE
 */
void halCpuHalt(CPU_HALT_MODE mode);

#endif  /* !_HAL_CPU_H_ */
