/*
 * ALGAE kernel string library
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _RTL_STRING_H_
#define _RTL_STRING_H_ 1

#include <ke/types.h>
#include <ke/vaargs.h>

/*
 * Returns the length of the string `str'
 * in bytes, exclusive of the null terminator
 */
USIZE rtlStrlen(const char *str);

/*
 * Convert a string into a signed 32-bit
 * integer
 */
LONG rtlToInt32(const char *s);

/*
 * Copy `src' to `dest' buffer with `n` bytes
 */
void *rtlMemcpy(void *dest, const void *src, USIZE n);

/*
 * Copy `n' instances of `c' to `s'
 */
void *rtlMemset(void *s, int c, USIZE n);

/*
 * Compare two arrays of bytes
 */
int rtlMemcmp(const void *s1, const void *s2, USIZE n);

/*
 * Copy a printf-style format string to a buffer
 */
int rtlBufPrintfV(char *buf, USIZE bufsize, const char *fmt, va_list va);

/*
 * Convert an integer to a string
 */
char *rtlItoa(LONG value, char *buf, int base);

#endif  /* !_RTL_STRING_H_ */
