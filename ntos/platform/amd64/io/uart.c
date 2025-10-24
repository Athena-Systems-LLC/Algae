/*
 * ALGAE 165XX UART driver
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <hal/uart.h>
#include <hal/pio.h>
#include <md/io/uart.h>
#include <ke/types.h>

/*
 * Output a single byte to a UART serial
 * port
 */
int
halUartWrite(CHAR byte, USHORT flags)
{
    halBusOutb(UART_REG_THR, byte);
    return byte;
}

/*
 * Initialize the UART driver
 */
int
halUartInit(void)
{
    /* Disable interrupts and set DLAB */
    halBusOutb(UART_REG_IER, 0x00);
    halBusOutb(UART_REG_LCR, UART_LCR_DLAB);

    /* Set buad rate */
    halBusOutb(UART_REG_DLL, UART_DIVISOR(57600));
    halBusOutb(UART_REG_IER, 0x00);

    /* Set word size to 8 bits and clear DLAB */
    halBusOutb(UART_REG_LCR, UART_LCR_WLS0 | UART_LCR_WLS1);

    /* Disable FIFOs */
    halBusOutb(UART_REG_FCR, 0x00);

    /* Test chip in loopback mode */
    halBusOutb(UART_REG_MCR, UART_MCR_LOOP);
    halBusOutb(UART_REG_THR, 0xF0);
    if (halBusInb(UART_REG_RBR != 0xF0)) {
        return -1;
    }

    /* Mark data terminal ready */
    halBusOutb(UART_REG_MCR, UART_MCR_DTR);
    return 0;
}
