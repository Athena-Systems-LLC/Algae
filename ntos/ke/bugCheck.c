/*
 * ALGAE Bug check / panic core
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/bugCheck.h>
#include <ke/vaargs.h>
#include <rtl/string.h>
#include <drv/bootVid.h>
#include <hal/cpu.h>

#define BUG_CHECK_BG 0x021AE1
#define BUG_CHECK_FG 0xFFFFFF

NORETURN
void keBugCheck(const char *fmt, ...)
{
    va_list ap;
    static char buf[512];

    va_start(ap, fmt);
    bootVidClear(BUG_CHECK_BG);

    rtlBufPrintfV(buf, sizeof(buf), fmt, ap);
    bootVidPrint(
        0,
        0,
        BUG_CHECK_FG,
        BUG_CHECK_BG,
        "** BUG CHECK **"
    );

    /* Print a line after */
    bootVidPrint(
        0,
        FONT_HEIGHT,
        BUG_CHECK_FG,
        BUG_CHECK_BG,
        buf
    );

    halCpuHalt(CPU_HALT_SELF);
    __builtin_unreachable();
}
