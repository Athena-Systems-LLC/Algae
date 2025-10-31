/*
 * ALGAE Trapframe helper macros
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_FRAMEASM_H_
#define _MACHINE_FRAMEASM_H_ 1

#define PUSH_ALL    \
    push %r15      ;\
    push %r14      ;\
    push %r13      ;\
    push %r12      ;\
    push %r11      ;\
    push %r10      ;\
    push %r9       ;\
    push %r8       ;\
    push %rbp      ;\
    push %rdi      ;\
    push %rsi      ;\
    push %rbx      ;\
    push %rdx      ;\
    push %rcx      ;\
    push %rax      ;

#define POP_ALL                         ; \
    pop %rax                            ; \
    pop %rcx                            ; \
    pop %rdx                            ; \
    pop %rbx                            ; \
    pop %rsi                            ; \
    pop %rdi                            ; \
    pop %rbp                            ; \
    pop %r8                             ; \
    pop %r9                             ; \
    pop %r10                            ; \
    pop %r11                            ; \
    pop %r12                            ; \
    pop %r13                            ; \
    pop %r14                            ; \
    pop %r15

/*
 * If the interrupt has an error code, this macro shall
 * be used to create the trapframe.
 *
 * XXX: A trapframe created with this must be popped with
 *      pop_trapframe_ec
 */
#define PUSH_TRAPFRAME_EC(TRAPNO) \
    PUSH_ALL                      \
    push TRAPNO

/*
 * If the interrupt has an error code, this macro shall
 * be used to cleanup the trapframe.
 */
#define POP_TRAPFRAME_EC                  \
    add $8, %rsp        /* Trapno */    ; \
    POP_ALL                             ; \

/*
 * If the interrupt has no error code, this macro
 * shall be used to create the trapframe.
 *
 * XXX: A trapframe created with this must be popped
 *      with pop_trapframe
 */
#define PUSH_TRAPFRAME(TRAPNO)   \
    push $0                    ;\
    PUSH_TRAPFRAME_EC(TRAPNO)

/*
 * If the interrupt has no error code, this macro shall
 * be used to cleanup the trapframe.
 */
#define POP_TRAPFRAME    \
    POP_TRAPFRAME_EC   ;\
    add $8, %rsp

#define INTR_ENTRY(NAME)            \
    NAME:                           \
        testq $0x3, 8(%rsp)        ;\
        jz 1f                      ;\
        lfence                     ;\
        swapgs                     ;\
    1:  PUSH_TRAPFRAME($0)         ;\

#define INTR_EXIT(NAME)             \
    NAME##_exit:                    \
        POP_TRAPFRAME              ;\
        testq $0x3, 8(%rsp)        ;\
        jz 2f                      ;\
        lfence                     ;\
        swapgs                     ;\
    2:  iretq

#define TRAP_ENTRY_EC(NAME, TRAPNO) \
    NAME:                           \
        cli                        ;\
        testq $0x3, 16(%rsp)       ;\
        jz 1f                      ;\
        lfence                     ;\
        swapgs                     ;\
    1:  PUSH_TRAPFRAME_EC(TRAPNO)  ;\

#define TRAP_EXIT_EC(NAME)          \
    NAME##_exit:                   ;\
        POP_TRAPFRAME_EC           ;\
        testq $0x3, 16(%rsp)       ;\
        jz 2f                      ;\
        lfence                     ;\
        swapgs                     ;\
        sti                        ;\
    2:  iretq

#define TRAP_ENTRY(NAME, TRAPNO)    \
    NAME:                           \
        cli                        ;\
        testq $0x3, 8(%rsp)        ;\
        jz 1f                      ;\
        lfence                     ;\
        swapgs                     ;\
    1:  PUSH_TRAPFRAME(TRAPNO)     ;\

#define TRAP_EXIT(NAME)             \
        POP_TRAPFRAME_EC           ;\
        testq $0x3, 16(%rsp)       ;\
        jz 2f                      ;\
        lfence                     ;\
        swapgs                     ;\
        sti                        ;\
    2:  iretq

#endif  /* !_MACHINE_FRAMEASM_H_ */
