/*
 * ALGAE Trap handling core
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <md/trap.h>
#include <ke/types.h>
#include <ke/defs.h>
#include <ke/syscall.h>
#include <ke/types.h>
#include <ke/bugCheck.h>

static const char *trapTab[] = {
    [TRAP_NONE]         = "bad",
    [TRAP_BREAKPOINT]   = "breakpoint",
    [TRAP_ARITH_ERR]    = "arithmetic error",
    [TRAP_OVERFLOW]     = "overflow",
    [TRAP_BOUND_RANGE]  = "bound range exceeded",
    [TRAP_INVLOP]       = "invalid opcode",
    [TRAP_DOUBLE_FAULT] = "double fault",
    [TRAP_INVLTSS]      = "invalid TSS",
    [TRAP_SEGNP]        = "segment not present",
    [TRAP_PROTFLT]      = "general protection",
    [TRAP_PAGEFLT]      = "page fault",
    [TRAP_NMI]          = "non-maskable interrupt",
    [TRAP_SS]           = "stack-segment fault"
};

void
trap_syscall(struct trapFrame *tf)
{
    SYSCALL_ARGS args = {
        .arg[0] = tf->rdi,
        .arg[1] = tf->rsi,
        .arg[2] = tf->rdx,
        .arg[3] = tf->r10,
        .arg[4] = tf->r9,
        .arg[5] = tf->r8
    };

    args.code = tf->rax;
    if (args.code >= g_syscallCount) {
        return;
    }

    tf->rax = g_syscallTable[args.code](&args);
}

void
trap_handler(struct trapFrame *tf)
{
    const char *trapStr;

    if (tf->trapno > NELEM(trapTab)) {
        keBugCheck("bad trap type %x\n", tf->trapno);
    }

    trapStr = trapTab[tf->trapno];
    keBugCheck("got fatal trap: %s\n", trapStr);
}
