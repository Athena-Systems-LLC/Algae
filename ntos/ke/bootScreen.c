/*
 * ALGAE boot screen implementation
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <drv/bootVid.h>
#include <rtl/string.h>
#include <ke/assert.h>
#include <ke/bootParam.h>
#include <ke/boot.h>

#define BOOT_MSG_FG   0xFFFFFF
#define BOOT_MSG_BG   0x000000
#define BOOT_MSG_YPOS (fbParams->height / 2)
#define FOOT_MSG_YPOS (fbParams->height - FONT_HEIGHT)

/* Helper for centering text */
#define CENTER_XPOS(STR) \
    ((fbParams->width) / 2) - (rtlStrlen((STR)) * FONT_WIDTH) / 2

static struct fbParams *fbParams;
static struct bootParams bootParams;

void
kiBootScreen(void)
{
    char bootMsg[] = "Starting ALGAE...";
    char footMsg[] = "Copyright (c) 2025 Ian Moffett and the Algae team";
    int error;

    /* Should not fail */
    error = keGetBootParams(&bootParams, 0);
    ASSERT(error == 0);
    fbParams = &bootParams.fbParams;

    /* Draw the boot message */
    bootVidPrint(
        CENTER_XPOS(bootMsg),
        BOOT_MSG_YPOS,
        BOOT_MSG_FG,
        BOOT_MSG_BG,
        bootMsg
    );

    /* Draw the copyright footer */
    bootVidPrint(
        CENTER_XPOS(footMsg),
        FOOT_MSG_YPOS,
        BOOT_MSG_FG,
        BOOT_MSG_BG,
        footMsg
    );
}
