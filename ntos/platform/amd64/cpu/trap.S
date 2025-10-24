/*
 * ALGAE Trap handling ISR entrypoints
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <md/frameAsm.h>
#include <md/trap.h>

    .globl breakpoint_handler
TRAP_ENTRY(breakpoint_handler, $TRAP_BREAKPOINT)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT(breakpoint_handler)

    .globl arith_err
TRAP_ENTRY(arith_err, $TRAP_ARITH_ERR)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT(arith_err)

    .globl overflow
TRAP_ENTRY(overflow, $TRAP_OVERFLOW)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT(overflow)

    .globl bound_range
TRAP_ENTRY(bound_range, $TRAP_BOUND_RANGE)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT(bound_range)

    .globl invl_op
TRAP_ENTRY(invl_op, $TRAP_INVLOP)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT(invl_op)

    .globl double_fault
TRAP_ENTRY_EC(double_fault, $TRAP_DOUBLE_FAULT)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT_EC(double_fault)

    .globl invl_tss
TRAP_ENTRY_EC(invl_tss, $TRAP_INVLTSS)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT_EC(invl_tss)

    .globl segnp
TRAP_ENTRY_EC(segnp, $TRAP_SEGNP)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT_EC(segnp)

    .globl general_prot
TRAP_ENTRY_EC(general_prot, $TRAP_PROTFLT)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT_EC(general_prot)

    .globl page_fault
TRAP_ENTRY_EC(page_fault, $TRAP_PAGEFLT)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT_EC(page_fault)

    .globl nmi
TRAP_ENTRY(nmi, $TRAP_NMI)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT(nmi)

    .globl ss_fault
TRAP_ENTRY_EC(ss_fault, $TRAP_SS)
    mov %rsp, %rdi
    call trap_handler
TRAP_EXIT_EC(ss_fault)
