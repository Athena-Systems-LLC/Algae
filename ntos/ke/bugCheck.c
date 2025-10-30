/*
 * ALGAE Bug check / panic core
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/bugCheck.h>
#include <ke/vaargs.h>
#include <ke/bootParam.h>
#include <ke/spinlock.h>
#include <ke/kpcr.h>
#include <rtl/string.h>
#include <drv/bootVid.h>
#include <hal/cpu.h>
#include <hal/bugCheck.h>

static KSPIN_LOCK lock;
static struct bootParams params;
static struct fbParams *fbParams;
static ULONG curLogLine = 4;

static void
bugCheckDump(const char *fmt, va_list diag_ap)
{
    static va_list ap;
    static char fmtbuf[256];
    static char heading[32];
    static char msg[256];
    static KPCR *kpcr;

    kpcr = keGetCore();

    /* Print the heading */
    kiBugCheckPrint(
        HEADING_FG,
        HEADING_BG,
        "ALGAE BUGCHECK"
    );

    /* Now the body */
    ++curLogLine;
    kiBugCheckPrint(
        BODY_FG,
        BODY_BG,
        "A fatal error has occured and the system has been halted "
        "to prevent data loss or corruption"
    );

    /* Some bug reporting information */
    kiBugCheckPrint(
        BODY_FG,
        BODY_BG,
        "Please report this bug to ian@osmora.org"
    );

    /* Prep the actual error message */
    rtlBufPrintfV(
        fmtbuf,
        sizeof(fmtbuf),
        fmt,
        diag_ap
    );

    ++curLogLine;
    kiBugCheckPrint(
        DIAG_FG,
        DIAG_BG,
        fmtbuf
    );

    /* Print the current cpu if possible */
    if (kpcr != NULL) {
        kiBugCheckPrint(
            DIAG_FG,
            DIAG_BG,
            "occured on cpu%d\n",
            kpcr->vId
        );
    }

    ++curLogLine;
    halDumpStack();
}

void
kiBugCheckPrint(ULONG fg, ULONG bg, const char *fmt, ...)
{
    va_list ap;
    static char msg[256];

    va_start(ap, fmt);
    rtlBufPrintfV(
        msg,
        sizeof(msg),
        fmt,
        ap
    );

#define CENTER_XPOS(STR) \
    ((fbParams->width) / 2) - (rtlStrlen((STR)) * FONT_WIDTH) / 2

    /* Print the heading */
    bootVidPrint(
        CENTER_XPOS(msg),
        curLogLine * FONT_HEIGHT,
        fg,
        bg,
        msg
    );
#undef CENTER_XPOS
    ++curLogLine;
    va_end(ap);
}

NORETURN
void keBugCheck(const char *fmt, ...)
{
    va_list ap;

    keAcquireSpinLock(&lock);   /* Never release */
    va_start(ap, fmt);
    keGetBootParams(&params, 0);
    fbParams = &params.fbParams;

    bootVidClear(BUG_CHECK_BG);
    bugCheckDump(fmt, ap);

    halCpuHalt(CPU_HALT_SELF);
    __builtin_unreachable();
}
