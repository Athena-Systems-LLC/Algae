/*
 * ALGAE Trapframe definitions macros
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */


#ifndef _MACHINE_FRAME_H_
#define _MACHINE_FRAME_H_ 1

#include <ke/types.h>

/*
 * Every time we are trapped into the kernel due
 * to an interrupt, a trapframe will be created
 * storing the current processor register state
 * so that it may be restored later.
 */
struct trapFrame {
    UQUAD trapno;
    UQUAD rax;
    UQUAD rcx;
    UQUAD rdx;
    UQUAD rbx;
    UQUAD rsi;
    UQUAD rdi;
    UQUAD rbp;
    UQUAD r8;
    UQUAD r9;
    UQUAD r10;
    UQUAD r11;
    UQUAD r12;
    UQUAD r13;
    UQUAD r14;
    UQUAD r15;

    /* pushed by hardware */
    UQUAD error_code;
    UQUAD rip;
    UQUAD cs;
    UQUAD rflags;
    UQUAD rsp;
    UQUAD ss;
};

#endif /* _MACHINE_FRAME_H_ */
