/*
 * ALGAE low level logging facility
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/trace.h>
#include <hal/uart.h>

void
keSerialTrace(const char *s, USIZE len)
{
    for (USIZE i = 0; i < len; ++i) {
        halUartWrite(s[i], 0);
    }
}
