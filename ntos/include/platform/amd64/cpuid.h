/*
 * ALGAE CPUID helpers
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_CPUID_H_
#define _MACHINE_CPUID_H_ 1

#include <ke/defs.h>

#define CPUID(level, a, b, c, d)                    \
    ASMV(                                           \
        "cpuid\n\t"                                 \
        : "=a" (a), "=b" (b), "=c" (c), "=d" (d)    \
        : "0" (level)                               \
    )

#endif  /* !_MACHINE_CPUID_H_ */
