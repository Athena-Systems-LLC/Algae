/*
 * ALGAE executive logging service
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/trace.h>
#include <ke/types.h>
#include <rtl/string.h>

void
exTraceLn(USHORT level, const char *ln)
{
    USIZE lnLen;

    if (ln == NULL) {
        return;
    }

    lnLen = rtlStrlen(ln);
    keSerialTrace(ln, lnLen);
}
