/*
 * ALGAE kernel string library
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <rtl/string.h>
#include <ke/types.h>

USIZE
rtlStrlen(CHAR *str)
{
    USIZE i = 0;

    if (str == NULL) {
        return 0;
    }

    while (str[i++] != '\0');
    return i - 1;
}
