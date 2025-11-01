/*
 * ALGAE System Call definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_SYSCALL_H_
#define _KE_SYSCALL_H_ 1

#include <ke/types.h>

#define NT_SYS_NONE  0x00
#define NT_SYS_BALL  0x01

typedef UQUAD SCRET;

typedef struct {
    UQUAD code;
    QUAD arg[6];
} SYSCALL_ARGS;

extern SCRET(*g_syscallTable[])(SYSCALL_ARGS *args);
extern const USIZE g_syscallCount;

#endif  /* !_KE_SYSCALL_H_ */
