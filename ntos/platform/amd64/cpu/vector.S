/*
 * ALGAE generic interrupt vectors
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <md/frameAsm.h>
#include <md/trap.h>

    .text
    .globl lapic_tmr_isr
INTR_ENTRY(lapic_tmr_isr)
    mov %rsp, %rdi
    call halSchedSwitch
INTR_EXIT(lapic_tmr_isr)

    .globl syscall_handler
syscall_handler:
    lfence               // Serialize loads [swapgs spectre mitigation]
    swapgs               // Get the kernel %GS base

    movq %rsp, %gs:8     // Save old the stack           [SYSCALL STACK SAVE]
    movq %gs:0, %rsp     // Grab the system call stack   [SYSCALL STACK BASE]
    PUSH_ALL             // Save all our gpregs

    // ...
    // XXX: CALL A SYSCALL TRAP HANDLER HERE
    // ...

    POP_ALL              // Restore our gpregs
    movq %gs:8, %rsp     // Restore the stack pointer
    movq $0, %gs:8       // Clear the old value for security

    lfence               // Serialize that shit
    swapgs               // Switch back to user TLS or whatever shit
    sysret
