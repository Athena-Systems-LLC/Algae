/*
 * Stdarg implementation
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_VAARGS_H_
#define _KE_VAARGS_H_ 1

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif      /* __GNUC_VA_LIST */

typedef __gnuc_va_list va_list;

#define va_start(ap, last)  __builtin_va_start((ap), last)
#define va_end(ap)          __builtin_va_end((ap))
#define va_arg(ap, type)    __builtin_va_arg((ap), type)

#endif  /* !_KE_VAARGS_H_ */
