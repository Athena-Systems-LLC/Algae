/*
 * ALGAE UART166XX driver definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _IO_UART_H_
#define _IO_UART_H_ 1

#include <ke/defs.h>

/* Channel port numbers */
#define UART_COM1 0x3F8
#define UART_COM2 0x2F8
#define UART_COM3 0x3E8
#define UART_COM4 0x2E8
#define UART_COM5 0x5F8
#define UART_COM6 0x4F8
#define UART_COM7 0x5E8
#define UART_COM8 0x4E8

/* Register offsets */
#define UART_REG(OFFSET) (UART_COM1 + OFFSET)
#define UART_REG_FCR UART_REG(2) /* FIFO Control Register */
#define UART_REG_LCR UART_REG(3) /* Line Control Register */
#define UART_REG_MCR UART_REG(4) /* MODEM Control Register */
#define UART_REG_LSR UART_REG(5) /* Line Status Register */
#define UART_REG_MSR UART_REG(6) /* MODEM Status Register */
#define UART_REG_SR  UART_REG(7) /* Scratch Register */

/* Registers when LCR.DLAB=0 */
#define UART_REG_RBR UART_REG(0) /* Receiver Buffer Register */
#define UART_REG_THR UART_REG(0) /* Transmitter Holding Register */
#define UART_REG_IER UART_REG(1) /* Interrupt Enable Register */

/* Registers when LCR.DLAB=1 */
#define UART_REG_DLL UART_REG(0) /* Divisor Latch Low */
#define UART_REG_DLH UART_REG(1) /* Divisor Latch High */

#define UART_LCR_WLS0 BIT(0) /* Word Length Select Bit 0 */
#define UART_LCR_WLS1 BIT(1) /* Word Length Select Bit 1 */
#define UART_LCR_DLAB BIT(7) /* Divisor Latch Access Bit*/

#define UART_MCR_DTR  BIT(0) /* Data Terminal Ready*/
#define UART_MCR_LOOP BIT(4) /* Loop */

#define UART_LSR_THRE BIT(5) /* Transmitter Holding Register */

#define UART_DIVISOR(RATE) (115200 / RATE)

#endif  /* _IO_UART_H_ */
