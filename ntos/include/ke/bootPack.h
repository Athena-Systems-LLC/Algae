/*
 * ALGAE Boot Pack Definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_BOOTPACK_H_
#define _KE_BOOTPACK_H_ 1

#include <ke/types.h>
#include <ke/defs.h>

/*
 * Initialize the bootpack subsystem so that early
 * required files can be obtained.
 */
void kiBootPackInit(void);

/*
 * Lookup a file from the bootpack using a path
 *
 * @path: Path to lookup
 */
CHAR *keBootPackGet(const CHAR *path);

#endif  /* !_KE_BOOTPACK_H_ */
