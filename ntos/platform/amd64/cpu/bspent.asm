;;
;; ALGAE Bootstrap Processor Entry
;; Copyright (c) 2025, Ian Moffett and the Algae team.
;; All rights reserved.
;; Provided under the BSD 3-Clause license.
;;

%define ALIGN    (1 << 0) ;; Align modules
%define MEMINFO  (1 << 1) ;; Get memory map
%define FLAGS    (ALIGN | MEMINFO)
%define MAGIC    0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

extern kMain

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_base: resb 4096  ;; 4K sized stack
stack_top:

section .text
global _start
_start:
    cli
    mov esp, stack_top

    jmp kMain
