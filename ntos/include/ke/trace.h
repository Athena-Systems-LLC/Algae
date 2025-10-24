/*
 * ALGAE low level logging facility
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_LOG_H_
#define _KE_LOG_H_

#include <ke/types.h>

/*
 * Log a message to the main serial port
 *
 * @s: Message to log
 * @len: Length of message
 */
void keSerialTrace(const char *s, USIZE len);

#endif  /* !_KE_LOG_H_ */
