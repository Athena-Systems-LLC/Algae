/*
 * ALGAE Memory Mapped I/O helpers
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_MMIO_H_
#define _KE_MMIO_H_ 1

#include <ke/types.h>
#include <ke/defs.h>

#define _MMIO_MAKE_WRITE(BIT_SIZE, TYPE)        \
    ALWAYS_INLINE static inline void            \
    mmioWrite##BIT_SIZE(TYPE *ADDR, TYPE val)   \
    {                                           \
        *(volatile TYPE *)ADDR = val;           \
        ASMV("" ::: "memory");                  \
    }

#define _MMIO_MAKE_READ(BIT_SIZE, TYPE)         \
    ALWAYS_INLINE static inline TYPE            \
    mmioRead##BIT_SIZE(TYPE *ADDR)              \
    {                                           \
        ASMV("" ::: "memory");                  \
        return *(volatile TYPE *)ADDR;          \
    }

/*
 * static void mmioWrite<n>(PTR, VAL)
 */
_MMIO_MAKE_WRITE(8, UCHAR);
_MMIO_MAKE_WRITE(16, USHORT);
_MMIO_MAKE_WRITE(32, ULONG);
#if __SIZEOF_SIZE_T__ == 8
_MMIO_MAKE_WRITE(64, UQUAD);
#endif

/*
 * static void mmioRead<n>(PTR)
 */
#endif  /* !_KE_MMIO_H_ */
_MMIO_MAKE_READ(8, UCHAR);
_MMIO_MAKE_READ(16, USHORT);
_MMIO_MAKE_READ(32, ULONG);
#if __SIZEOF_SIZE_T__ == 8
_MMIO_MAKE_READ(64, UQUAD);
#endif
