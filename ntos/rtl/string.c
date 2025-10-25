/*
 * ALGAE kernel string library
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <rtl/string.h>
#include <ke/types.h>
#include <ke/vaargs.h>

#define IS_DIGIT(C) ((C >= '0' && C <= '9'))

void *
rtlMemcpy(void *dest, const void *src, USIZE n)
{
    for (USIZE i = 0; i < n; ++i) {
        ((char *)dest)[i] = ((char *)src)[i];
    }

    return dest;
}

/*
 * Alias of rtlMemset() so compilers don't
 * complain with certain things.
 */
void *
memset(void *s, int c, unsigned long n)
{
    return rtlMemset(s, c, n);
}

void *rtlMemset(void *s, int c, USIZE n) {
    for (USIZE i = 0; i < n; ++i) {
        ((char *)s)[i] = c;
    }

    return s;
}

static inline void
printC(char *buf, USIZE size, USIZE *off, char c)
{
    if (*off < size - 1) {
        buf[(*off)++] = c;
    }
    buf[*off] = 0;
}

static void
printStr(char *buf, USIZE size, USIZE *off, const char *s)
{
    while (*off < size - 1 && *s != '\0') {
        buf[(*off)++] = *(s)++;
    }
    buf[*off] = 0;
}

int
rtlBufPrintf(char *s, USIZE size, const char *fmt, ...)
{
    va_list ap;
    int ret;

    va_start(ap, fmt);
    ret = rtlBufPrintfV(s, size, fmt, ap);
    va_end(ap);
    return ret;
}

int
rtlBufPrintfV(char *s, USIZE size, const char *fmt, va_list ap)
{
    USIZE tmpLen, num_len, off = 0;
    USIZE num = 0;
    char c, c1, numBuf[256] = {0};
    const char *tmpStr;
    UCHAR pad_width = 0;

    while (off < (size - 1)) {
        while (*fmt && *fmt != '%') {
            printC(s, size, &off, *fmt++);
        }
        if (*(fmt)++ == '\0' || off == size - 1) {
            return off;
        }

        /*
         * Handle a case where we have "%0N". For example:
         * "%04d"
         */
        if (*fmt == '0') {
            ++fmt;
            while (*fmt >= '0' && *fmt <= '9') {
                pad_width = pad_width * 10 + (*fmt - '0');
                ++fmt;
            }
        }

        c = *fmt++;
        switch (c) {
        case 'c':
            c1 = (char )va_arg(ap, int);
            printC(s, size, &off, c1);
            break;
        case '%':
            printC(s, size, &off, c);
            break;
        case 'x':
            num = va_arg(ap, UQUAD);
            rtlItoa(num, numBuf, 16);
            tmpLen = rtlStrlen(numBuf);
            printStr(s, size, &off, numBuf + 2);
            break;
        case 'd':
        case 'u':
            if (c == 'd') {
                num = va_arg(ap, LONG);
            } else {
                num = va_arg(ap, ULONG);
            }

            rtlItoa(num, numBuf, 10);
            if (pad_width > 0) {
                num_len = rtlStrlen(numBuf);
                for (USIZE i = num_len; i < pad_width; ++i)
                    printC(s, size, &off, '0');
                pad_width = 0;
            }
            printStr(s, size, &off, numBuf);
            break;
        case 's':
            tmpStr = va_arg(ap, const char *);
            printStr(s, size, &off, tmpStr);
            break;
        }
    }

    return 0;
}

USIZE
rtlStrlen(const char *str)
{
    USIZE i = 0;

    if (str == NULL) {
        return 0;
    }

    while (str[i++] != '\0');
    return i - 1;
}

LONG
rtlToInt32(const char *s)
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
