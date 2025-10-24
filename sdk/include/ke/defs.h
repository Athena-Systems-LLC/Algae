/*
 * ALGAE Kernel C definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_DEFS_H_
#define _KE_DEFS_H_ 1

/* Attribute macros */
#define PACKED          __attribute__((__packed__))
#define ALWAYS_INLINE   __attribute__((__always_inline__))

/* Bitwise helpers */
#define BIT(n) (1ULL << (n))

/* Misc macros */
#define ASMV __asm__ __volatile__

#endif  /* !_KE_DEFS_H_ */
