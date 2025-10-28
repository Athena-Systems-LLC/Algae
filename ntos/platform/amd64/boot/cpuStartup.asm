;;
;; ALGAE CPU startup trampoline
;; Copyright (c) 2025, Ian Moffett and the Algae team.
;; All rights reserved.
;; Provided under the BSD 3-Clause license.
;;

[bits 16]
[org 0x0000]

start:
    cli
    hlt
