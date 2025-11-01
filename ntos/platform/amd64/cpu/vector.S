/*
 * ALGAE generic interrupt vectors
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <md/frameAsm.h>
#include <md/trap.h>

    .text
    .globl lapicTmrIsr
INTR_ENTRY(lapicTmrIsr)
    mov %rsp, %rdi
    call halSchedSwitch
INTR_EXIT(lapicTmrIsr)

    .globl syscallHandler
syscallHandler:
    lfence               // Serialize loads [swapgs spectre mitigation]
    swapgs               // Get the kernel %GS base

    movq %rsp, %gs:8     // Save old the stack           [SYSCALL STACK SAVE]
    movq %gs:0, %rsp     // Grab the system call stack   [SYSCALL STACK BASE]
    PUSH_ALL             // Save all our gpregs
    sub $8, %rsp         // Skip error code

    mov %rsp, %rdi
    call trap_syscall

    add $8, %rsp         // Undo stack subtraction
    POP_ALL              // Restore our gpregs
    movq %gs:8, %rsp     // Restore the stack pointer
    movq $0, %gs:8       // Clear the old value for security

    lfence               // Serialize that shit
    swapgs               // Switch back to user TLS or whatever shit
    sysret
