/*
 * ALGAE boot video driver implementation
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/bootParam.h>
#include <ke/defs.h>
#include <ke/module.h>
#include <ke/bugCheck.h>
#include <ex/mapper.h>
#include <ob/object.h>
#include <rtl/string.h>
#include <drv/bootVid.h>
#include <mm/vm.h>
#include <ntstatus.h>

static NT_OBJECT *fbMapper;
static MAPPER_OBJECT mapperObj;
extern UCHAR g_consFont[];

/*
 * Get the physical framebuffer base
 * address
 */
static ULONG_PTR
bootVidGetBase(void)
{
    struct bootParams params;
    struct fbParams *fbParams;
    NTSTATUS status;

    status = keGetBootParams(&params, 0);
    if (status != STATUS_SUCCESS) {
        return 0;
    }

    fbParams = &params.fbParams;
    return (ULONG_PTR)fbParams->io - keGetKernelBase();
}

static NTSTATUS
bootVidInit(void)
{
    ULONG_PTR fbBase;
    MAPPER_REGION *region;
    NTSTATUS status;
    NT_OBJECT_CREATE params = {
        .parent = "/mapper",
        .name = "video0",
        .type = NT_OB_MAPPER
    };

    fbBase = bootVidGetBase();
    if (fbBase == 0) {
        keBugCheck("failed to initialize fb mapper region\n");
    }

    /* Initialize the mapper object */
    region = &mapperObj.region;
    mapperObj.type = MAPPER_FIXED;
    region->vBase = bootVidGetBase();
    region->pBase = region->vBase;
    region->prot = PAGE_USER_READWRITE;

    /* We need to register the framebuffer */
    status = obCreateObject(&params, &fbMapper);
    if (status != STATUS_SUCCESS) {
        keBugCheck("failed to register framebuffer\n");
    }

    fbMapper->data = &mapperObj;
    return STATUS_SUCCESS;
}

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
    NTSTATUS status;
    PSF_FONT *font = (PSF_FONT *)g_consFont;
    ULONG x, y, color;
    ULONG idx, *fbio;

    if (chp == NULL) {
        return -1;
    }

    status = keGetBootParams(&params, 0);
    if (status != STATUS_SUCCESS) {
        return status;
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
    ch.x = x;

    strLen = rtlStrlen(str);
    for (ULONG i = 0; i < strLen; ++i) {
        ch.ch = str[i];
        if (ch.ch == '\n') {
            ch.y += FONT_HEIGHT;
            ch.x = x;
            continue;
        }

        bootVidDrawCh(&ch);
        ch.x += FONT_WIDTH;
    }

    return 0;
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

MODULE_EXPORT("bootvid", bootVidInit, MODULE_GENERIC);
