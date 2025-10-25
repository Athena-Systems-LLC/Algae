/*
 * ALGAE boot video driver interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _DRV_BOOTVID_H_
#define _DRV_BOOTVID_H_ 1

#include <ke/types.h>
#include <ke/defs.h>

#define FONT_HDRLEN (sizeof(PSF_FONT))
#define FONT_WIDTH 8
#define FONT_HEIGHT 20

/*
 * Represents a single character that can be
 * drawn onto the screen
 */
typedef struct {
    ULONG x;
    ULONG y;
    ULONG fg;
    ULONG bg;
    CHAR ch;
} BOOTVID_CHAR;

/*
 * Represents a PC screen font used by
 * default on startup
 */
typedef struct {
    USHORT magic;
    UCHAR fontMode;
    UCHAR chSize;
} PSF_FONT;

/*
 * Clear the screen by filling it with an RGB pixel
 * described by `rgb'
 *
 * @rgb: RGB pixel to fill screen with
 */
void bootVidClear(ULONG rgb);

/*
 * Draw a character onto the screen and return
 * zero on success
 *
 * @chp: Character descriptor to draw
 */
int bootVidDrawCh(BOOTVID_CHAR *chp);

/*
 * Print a string to the screen
 *
 * @x: X position
 * @y: Y position
 * @fg: Foreground color
 * @bg: Background color
 * @str: String to render
 *
 * XXX: `str' must be NULL terminated
 *
 * Returns zero on success
 */
int bootVidPrint(
    ULONG x, ULONG y, ULONG fg,
    ULONG bg, const char *str
);

#endif  /* !_DRV_BOOTVID_H_ */
