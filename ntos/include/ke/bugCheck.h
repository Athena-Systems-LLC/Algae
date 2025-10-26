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

#define BUG_CHECK_BG 0x021AE1
#define BUG_CHECK_FG 0xFFFFFF

/* Heading parameters */
#define HEADING_FG BUG_CHECK_BG
#define HEADING_BG 0xAAAAAA

/* Body parameters */
#define BODY_FG BUG_CHECK_FG
#define BODY_BG BUG_CHECK_BG

/* Diagnosis parameters */
#define DIAG_FG BODY_FG
#define DIAG_BG BODY_BG

/*
 * Used to print to the bugcheck screen, used
 * internally by MD hooks
 */
void kiBugCheckPrint(ULONG fg, ULONG bg, const char *fmt, ...);

/*
 * Indicate to the user that something serious happened
 * in the system and it needs to be brought down
 */
NORETURN void keBugCheck(const char *fmt, ...);

#endif  /* !_KE_BUGCHECK_H_ */
