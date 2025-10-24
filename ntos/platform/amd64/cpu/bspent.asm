;;
;; ALGAE Bootstrap Processor Entry
;; Copyright (c) 2025, Ian Moffett and the Algae team.
;; All rights reserved.
;; Provided under the BSD 3-Clause license.
;;

[bits 32]

extern kMain

global _start
section .text
_start:
    jmp kMain
