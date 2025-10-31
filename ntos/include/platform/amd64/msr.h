/*
 * ALGAE AMD64 MSR helpers
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_MSR_H_
#define _MACHINE_MSR_H_ 1

#include <ke/types.h>
#include <ke/defs.h>

#define IA32_SPEC_CTL       0x00000048
#define IA32_EFER           0xC0000080
#define IA32_STAR           0xC0000081
#define IA32_LSTAR          0xC0000082
#define IA32_CSTAR          0xC0000083
#define IA32_SFMASK         0xC0000084
#define IA32_KERNEL_GS_BASE 0xC0000102
#define IA32_GS_BASE        0xC0000101
#define IA32_FS_BASE        0xC0000100
#define IA32_APIC_BASE_MSR  0x0000001B

ALWAYS_INLINE static inline UQUAD
rdmsr(ULONG msrAddr)
{
    ULONG lo, hi;

    ASMV("rdmsr"
         : "=a" (lo), "=d" (hi)
         : "c" (msrAddr)
    );
    return ((UQUAD)hi << 32) | lo;
}

ALWAYS_INLINE static inline void
wrmsr(ULONG msrAddr, UQUAD value)
{
    ULONG lo, hi;

    lo = (ULONG)value;
    hi = (ULONG)(value >> 32);

    ASMV("wrmsr"
         :  /* No outputs */
         : "a" (lo), "d" (hi),
           "c" (msrAddr)
    );
}

#endif  /* !_MACHINE_MSR_H_ */
