/*
 * ALGAE Kernel type definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_TYPES_H_
#define _KE_TYPES_H_ 1

#if defined(__cpluscplus)
#define NULL nullptr
#else
#define NULL (void *)0
#endif

typedef _Bool           BOOLEAN;

typedef unsigned char   UCHAR;
typedef char            CHAR;

typedef unsigned short  USHORT;
typedef short           SHORT;

typedef unsigned long   ULONG;
typedef long            LONG;

typedef unsigned long long  UQUAD;
typedef long long           QUAD;

typedef UQUAD USIZE;
typedef UQUAD ULONG_PTR;

#endif  /* !_KE_TYPES_H_ */
