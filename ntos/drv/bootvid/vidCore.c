/*
 * ALGAE boot video driver implementation
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/types.h>
#include <ke/bootParam.h>
#include <drv/bootVid.h>

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
