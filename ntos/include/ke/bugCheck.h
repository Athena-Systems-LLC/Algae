/*
 * ALGAE Bug check / panic core
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_BUGCHECK_H_
#define _KE_BUGCHECK_H_

#include <ke/types.h>
#include <ke/defs.h>

/*
 * Indicate to the user that something serious happened
 * in the system and it needs to be brought down
 */
NORETURN void keBugCheck(const char *fmt, ...);

#endif  /* !_KE_BUGCHECK_H_ */
