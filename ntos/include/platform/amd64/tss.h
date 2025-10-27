/*
 * ALGAE Task state segment
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_TSS_H_
#define _MACHINE_TSS_H_

#include <ke/types.h>
#include <ke/defs.h>
#include <ke/kpcr.h>
#include <ntstatus.h>

/* IST indices */
#define IST_SCHED 0x00

/*
 * Represents an interrupt stack
 */
typedef union {
    struct {
        ULONG stackLow;
        ULONG stackHigh;
    };
    UQUAD stackBase;
} TSS_STACK;

/*
 * Load the TSS
 *
 * XXX: Must be written first with tssWrite()
 */
ALWAYS_INLINE static inline void
tssLoad(void)
{
    /*
     * Store the old task register so that we can
     * overwrite it with the new segment selector
     * of the current TSS descriptor.
     */
    ASMV(
        "str %ax\n"
        "mov $0x2B, %ax\n"
        "ltr %ax"
    );
}

/*
 * Allocate an interrupt stack
 *
 * @result: Result is written here
 */
NTSTATUS tssAllocateStack(TSS_STACK *result);

/*
 * Set a specific interrupt stack table entry
 *
 * @kpcr: Current processor control region
 * @istNumber: Index into interrupt stack table
 * @stack: Stack to set
 */
NTSTATUS tssSetIntrStack(
    KPCR *kpcr, UCHAR istNumber,
    TSS_STACK *stack
);

/*
 * Write to the TSS descriptor and point it to the
 * task state segment
 */
void tssWrite(KPCR *kpcr, TSS_DESC *desc);

#endif  /* !_MACHINE_TSS_H_ */
