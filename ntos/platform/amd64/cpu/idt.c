/*
 * ALGAE AMD64 IDT implementation.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <md/idt.h>
#include <md/gdt.h>

static struct idtGate idt[256];
static struct idtr idtr = {
    .limit = sizeof(struct idtGate) * 256 - 1,
    .offset = (ULONG_PTR)&idt[0]
};

void
kiIdtSetEntry(UCHAR vec, UCHAR type, ULONG_PTR isr, UCHAR ist)
{
    struct idtGate *gate;

    if (vec >= 255) {
        return;
    }

    gate = &idt[vec];
    gate->offsetLow = isr & 0xFFFF;
    gate->offsetMid =  (isr >> 16) & 0xFFFF;
    gate->offsetHigh = (isr >> 32) & 0xFFFFFFFF;
    gate->ist = ist;
    gate->zero = 0;
    gate->zero1 = 0;
    gate->reserved = 0;
    gate->dpl = (type == IDT_USER_GATE) ? 3 : 0;
    gate->p = 1;
    gate->type = type;
    gate->codeSeg = KERNEL_CS;
}

void
kiIdtLoad(void)
{
    ASMV(
        "lidt %0"
        :
        : "m" (idtr)
        : "memory"
    );
}
