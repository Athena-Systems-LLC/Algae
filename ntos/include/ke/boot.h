/*
 * ALGAE Internal boot up routines
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_BOOT_H_
#define _KE_BOOT_H_ 1

int kiPlatformInit(void);
int kiProcessorInit(void);
void kiBootScreen(void);

#endif  /* !_KE_BOOT_H_ */
