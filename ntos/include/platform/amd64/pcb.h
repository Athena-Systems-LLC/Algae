/*
 * ALGAE AMD64 process control block
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_PCB_H_
#define _MACHINE_PCB_H_

#include <ke/types.h>
#include <md/frame.h>
#include <md/vas.h>

/*
 * Process control block structure
 *
 * @vas: Virtual address space
 * @stackPhysBase: Stack physical base address
 * @tf: Trapframe store
 */
typedef struct {
    MMU_VAS vas;
    UQUAD stackPhysBase;
    struct trapFrame tf;
} PCB;

#endif  /* !_MACHINE_PCB_H_ */
