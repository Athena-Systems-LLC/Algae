/*
 * ALGAE AMD64 I/O Bus helpers
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

    .text
    .globl halBusOutb
halBusOutb:
    mov %rdi, %rdx      // PORT
    mov %rsi, %rax      // VAL
    outb %al, %dx
    retq

    .globl halBusOutw
halBusOutw:
    mov %rdi, %rdx      // PORT
    mov %rsi, %rax      // VAL
    outw %ax, %dx
    retq

    .globl halBusOutl
halBusOutl:
    mov %rdi, %rdx      // PORT
    mov %rsi, %rax      // VAL
    outl %eax, %dx
    retq

    .globl halBusInb
halBusInb:
    mov %rdi, %rdx      // PORT
    xor %rax, %rax
    inb %dx, %al
    retq

.global halBusInw
halBusInw:
    mov %rdi, %rdx      // PORT
    xor %rax, %rax
    inw %dx, %ax
    retq

.global halBusInl
halBusInl:
    mov %rdi, %rdx      // PORT
    xor %rax, %rax
    inl %dx, %eax
    retq
