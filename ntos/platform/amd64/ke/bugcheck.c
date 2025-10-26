/*
 * ALGAE MD bug check helpers
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/bugCheck.h>
#include <ke/defs.h>
#include <ke/types.h>
#include <hal/bugCheck.h>

#define MAX_FRAMES 8

extern struct {
	UQUAD addr;
	char *name;
} g_ksymTbl[];

static const char *
stackAddrToName(ULONG_PTR addr)
{
    const char *name = "???";

    for (USIZE i = 0;;) {
        if (g_ksymTbl[i].addr > addr) {
            return name;
        }

        name = g_ksymTbl[i].name;
        if (g_ksymTbl[i++].addr == (ULONG_PTR)-1) {
            break;
        }
    }

    return "???";
}

void
halDumpStack(void)
{
    UQUAD *rbp, p;
    UQUAD rip, off;
    const char *name;

    ASMV(
        "mov %%rbp, %0"
        :
        : "m" (rbp)
        : "memory"
    );

    for (int i = 0; i < MAX_FRAMES; ++i) {
        rip = rbp[1];
        name = stackAddrToName(rip);

        kiBugCheckPrint(
            BUG_CHECK_FG,
            BUG_CHECK_BG,
            "[%d] %p - [%s]",
            i,
            rip,
            name
        );

        rbp = (UQUAD *)rbp[0];
        if (rbp == NULL || rip == 0) {
            break;
        }
    }
}
