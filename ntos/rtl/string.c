/*
 * ALGAE kernel string library
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <rtl/string.h>
#include <ke/types.h>

#define IS_DIGIT(C) ((C >= '0' && C <= '9'))

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

LONG
rtlToInt32(char *s)
{
    LONG n, sign;

    while (*s == ' ') {
        ++s;
    }

    sign = (*s == '-') ? -1 : 1;
    if (*s == '+' || *s == '-') {
        ++s;
    }
    for (n = 0; IS_DIGIT(*s); ++s) {
        n = 10 * n + (*s - '0');
    }

    return sign * n;
}
