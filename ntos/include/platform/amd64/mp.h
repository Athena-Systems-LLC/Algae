/*
 * ALGAE multiprocessor support
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_MP_H_
#define _MACHINE_MP_H_ 1

#include <ke/types.h>
#include <ke/defs.h>

/*
 * Begin bootstrapping the application
 * processors
 */
void kiMpInit(void);

#endif  /* !_MACHINE_MP_H_ */
