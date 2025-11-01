/*
 * ALGAE C runtime initialization
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

    .text
    .globl _start
_start:
    xor %rbp, %rbp          // Terminate callstack
    call __mainCrtStartup   // Initialize environment
    jmp .

    .globl __main
__main:
    nop
    retq
