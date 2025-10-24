/*
 * ALGAE AMD64 Trap definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_TRAP_H_
#define _MACHINE_TRAP_H_

#if !defined(__ASSEMBLER__)
#include <md/frame.h>
#endif

#define TRAP_NONE           0       /* Used for general interrupts */
#define TRAP_BREAKPOINT     1       /* Breakpoint */
#define TRAP_ARITH_ERR      2       /* Arithmetic error (e.g division by 0) */
#define TRAP_OVERFLOW       3       /* Overflow */
#define TRAP_BOUND_RANGE    4       /* BOUND range exceeded */
#define TRAP_INVLOP         5       /* Invalid opcode */
#define TRAP_DOUBLE_FAULT   6       /* Double fault */
#define TRAP_INVLTSS        7       /* Invalid TSS */
#define TRAP_SEGNP          8       /* Segment not present */
#define TRAP_PROTFLT        9       /* General protection */
#define TRAP_PAGEFLT        10      /* Page fault */
#define TRAP_NMI            11      /* Non-maskable interrupt */
#define TRAP_SS             12      /* Stack-segment fault */

#if !defined(__ASSEMBLER__)

void breakpoint_handler(void *sf);
void arith_err(void *sf);
void overflow(void *sf);
void bound_range(void *sf);
void invl_op(void *sf);
void double_fault(void *sf);
void invl_tss(void *sf);
void segnp(void *sf);
void general_prot(void *sf);
void page_fault(void *sf);
void nmi(void *sf);
void ss_fault(void *sf);
void trap_handler(struct trapFrame *tf);

#endif  /* !__ASSEMBLER__ */
#endif  /* !_MACHINE_TRAP_H_ */
