/*
 * ALGAE Kernel C definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_DEFS_H_
#define _KE_DEFS_H_ 1

#include <ke/types.h>

/* Attribute macros */
#define PACKED          __attribute__((__packed__))
#define ALWAYS_INLINE   __attribute__((__always_inline__))
#define NORETURN        __attribute__((__noreturn__))
#define WEAK            __attribute__((__weak__))
#define USED            __attribute__((__used__))
#define SECTION(X)      __attribute__((__section__((X)), __used__))

/* Min/max macros */
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

/* Bitwise helpers */
#define BIT(n) (1ULL << (n))
#define ISSET(v, f)  ((v) & (f))
#define MASK(n) ((1ULL << n) - 1)

/* Bitmap helper macros */
#define setBit(a, b) ((a)[(b) >> 3] |= BIT(b % 8))
#define clrBit(a, b) ((a)[(b) >> 3] &= ~BIT(b % 8))
#define testBit(a, b) (ISSET((a)[(b) >> 3], BIT(b % 8)))

/* Misc macros */
#define ASMV __asm__ __volatile__

/*
 * PTR_OFFSET: Adds an offset to the pointer
 * PTR_NOFFSET: Subtracts a negative offset from the pointer
 */
#define PTR_OFFSET(PTR, OFF) ((void *)((ULONG_PTR)PTR + OFF))
#define PTR_NOFFSET(PTR, NOFF) ((void *)((ULONG_PTR)PTR - NOFF))

/* Align up/down a value */
#define ALIGN_DOWN(value, align)      ((value) & ~((align)-1))
#define ALIGN_UP(value, align)        (((value) + (align)-1) & ~((align)-1))

/* Number of elements in an array */
#define NELEM(a) (sizeof(a) / sizeof(a[0]))

#endif  /* !_KE_DEFS_H_ */
