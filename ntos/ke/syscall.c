/*
 * ALGAE System Call core
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/syscall.h>
#include <ke/defs.h>
#include <ke/bugCheck.h>

SCRET(*g_syscallTable[])(SYSCALL_ARGS *args) = {
    [NT_SYS_NONE] = NULL,
};

const USIZE g_syscallCount = NELEM(g_syscallTable);
