/*
 * ALGAE AMD64 Virtual Address Space definition
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_VAS_H_
#define _MACHINE_VAS_H_

typedef struct {
    ULONG_PTR cr3;
} MMU_VAS;

#endif  /* !_MACHINE_VAS_H_ */
