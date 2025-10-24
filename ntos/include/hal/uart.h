/*
 * ALGAE Platform UART HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#include <ke/types.h>

/*
 * Output a single byte to a UART serial
 * port
 *
 * @byte: Byte to write to port
 * @flags: Optional flags
 *
 * Returns `byte' on success
 */
int halUartWrite(CHAR byte, USHORT flags);

/*
 * Initialize the UART driver
 *
 * Returns zero on success
 */
int halUartInit(void);

#endif  /* !_HAL_UART_H_ */
