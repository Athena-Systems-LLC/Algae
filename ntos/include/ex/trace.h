/*
 * ALGAE executive logging service
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _EX_TRACE_H_
#define _EX_TRACE_H_ 1

#include <ke/vaargs.h>
#include <ke/types.h>

#define EX_TRACE_INFO 0x0000

/*
 * Trace line to system logs
 *
 * @level: Log level
 * @ln: Line to trace
 */
void exTraceLn(USHORT level, const CHAR *ln);

#endif  /* !_EX_TRACE_H_ */
