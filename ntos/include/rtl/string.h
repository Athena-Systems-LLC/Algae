/*
 * ALGAE kernel string library
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _RTL_STRING_H_
#define _RTL_STRING_H_ 1

#include <ke/types.h>

/*
 * Returns the length of the string `str'
 * in bytes, exclusive of the null terminator
 */
USIZE rtlStrlen(CHAR *str);

#endif  /* !_RTL_STRING_H_ */
