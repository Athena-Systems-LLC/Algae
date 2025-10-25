/*
 * ALGAE boot video driver implementation
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/bootParam.h>
#include <ke/defs.h>
#include <rtl/string.h>
#include <drv/bootVid.h>

extern UCHAR g_consFont[];

/*
 * Get the index into the framebuffer with an x and y
 * position.
 *
 * @pitch: Framebuffer pitch
 * @x: X position to plot
 * @y: Y position to plot
 *
 * Returns index
 */
ALWAYS_INLINE static inline USIZE
fbGetIdx(ULONG pitch, ULONG x, ULONG y)
{
    return x + y * (pitch / 4);
}

int
bootVidDrawCh(BOOTVID_CHAR *chp)
{
    struct bootParams params;
    struct fbParams *fbParams;
    const UCHAR *glyph;
    PSF_FONT *font = (PSF_FONT *)g_consFont;
    ULONG x, y, color;
    ULONG idx, *fbio;
    int error;

    if (chp == NULL) {
        return -1;
    }

    error = keGetBootParams(&params, 0);
    if (error < 0) {
        return error;
    }

    fbParams = &params.fbParams;
    fbio = (ULONG *)fbParams->io;
    glyph = PTR_OFFSET(font, FONT_HDRLEN + chp->ch * font->chSize);
    x = chp->x;
    y = chp->y;

    for (USIZE cy = 0; cy < font->chSize; ++cy) {
        for (USIZE cx = 0; cx < 8; ++cx){
            color = ISSET(glyph[cy], BIT(8 - cx)) ? chp->fg : chp->bg;
            idx = fbGetIdx(fbParams->pitch, x+cx, y+cy);
            fbio[idx] = color;
        }
    }

    return 0;
}

int
bootVidPrint(ULONG x, ULONG y, ULONG fg, ULONG bg, const char *str)
{
    ULONG strLen;
    BOOTVID_CHAR ch;

    if (str == NULL) {
        return -1;
    }

    ch.fg = fg;
    ch.bg = bg;
    ch.y = y;

    strLen = rtlStrlen(str);
    for (ULONG i = 0; i < strLen; ++i) {
        ch.ch = str[i];
        if (ch.ch == '\n') {
            ch.y += FONT_HEIGHT;
            ch.x = 0;
            continue;
        }
        ch.x = x + (i * 8);
        bootVidDrawCh(&ch);
    }
}

void
bootVidClear(ULONG rgb)
{
    USIZE fbSize;
    struct bootParams params;
    struct fbParams *fbParams;
    int error;

    error = keGetBootParams(&params, 0);
    if (error < 0) {
        return;
    }

    fbParams = &params.fbParams;
    fbSize = fbParams->height * fbParams->pitch;

    for (USIZE i = 0; i < fbSize / 4; ++i) {
        fbParams->io[i] = rgb;
    }
}
