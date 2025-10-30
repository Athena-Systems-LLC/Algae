/*
 * ALGAE generic interrupt vectors
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <md/frameAsm.h>
#include <md/trap.h>

    .text
    .globl lapic_tmr_isr
INTR_ENTRY(lapic_tmr_isr)
    mov %rsp, %rdi
    call halSchedSwitch
INTR_EXIT(lapic_tmr_isr)
