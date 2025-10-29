;;
;; ALGAE CPU startup trampoline
;; Copyright (c) 2025, Ian Moffett and the Algae team.
;; All rights reserved.
;; Provided under the BSD 3-Clause license.
;;

[bits 16]
[org 0x1000]

%define BOOTSTRAP_DESC 0x2000

start:
    mov eax, 0b10100000                     ;; Enable PAE + paging
    mov cr4, eax                            ;; and write it to CR4

    mov ebx, dword [BOOTSTRAP_DESC]         ;; Get the bootstrap descriptor
    mov cr3, ebx                            ;; Set from PML4 PA field
    lidt [idtr]                             ;; Set the NULL IDTR

    mov ecx, 0xC0000080                     ;; IA32_EFER MSR base
    rdmsr                                   ;; Read the MSR
    or eax, 0xD00                           ;; Set LME + defaults for long mode
    wrmsr                                   ;; Write it back

    mov ebx, cr0                            ;; Enter long mode with
    or ebx, 0x80000001                      ;; CR0.PE+PG
    mov cr0, ebx                            ;; Write it

    lgdt [GDT.GDTR]                         ;; Load the flat descriptor
    jmp 0x8:longModeEntry                   ;; Long jump to entrypoint

;;
;; Zeroed IDTR so that the processor is in a known
;; state when bringing up
;;
align 4
idtr:
    .len dw 0
    .base dd 0

;;
;; Flat GDT
;;
GDT:
.NULL:
    dq 0x0000000000000000       ;; NULL DESCRIPTOR
.CODE:                          ;; ---------------
    dq 0x00209A0000000000       ;; CODE DESCRIPTOR
.DATA:                          ;; ---------------
    dq 0x0000920000000000       ;; DATA DESCRIPTOR
.GDTR:                          ;; ---------------
    dw $ - GDT - 1              ;; LIMIT
    dd GDT                      ;; OFFSET

;;
;; Long mode entrypoint
;;
[bits 64]
longModeEntry:
    mov ax, 0x10                            ;; DATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, 0x3000                         ;; Use 0x3000 as temp stack
    xor rbp, rbp                            ;; Terminate callstack

    mov rax, qword [BOOTSTRAP_DESC]
    mov cr3, rax

    mov rax, qword [BOOTSTRAP_DESC + 8]     ;; Get the trampoline address
    jmp rax

times 4096 - ($-$$) db 0
