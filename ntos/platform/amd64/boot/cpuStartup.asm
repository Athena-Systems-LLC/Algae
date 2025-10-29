;;
;; ALGAE CPU startup trampoline
;; Copyright (c) 2025, Ian Moffett and the Algae team.
;; All rights reserved.
;; Provided under the BSD 3-Clause license.
;;

[bits 16]
[org 0x1000]

start:
    cli
    hlt
    jmp start

times 4096 - ($-$$) db 0
