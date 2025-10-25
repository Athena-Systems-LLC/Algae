/*
 * ALGAE Boot up parameters
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/bootParam.h>
#include <ke/bugCheck.h>
#include <ke/types.h>
#include <rtl/limine.h>

#define FRAMEBUFFER \
    frameBufReq.response->framebuffers[0]

static ULONG_PTR hhmOffset = 0;

/* Request from bootloader */
static volatile struct limine_framebuffer_request frameBufReq = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
static volatile struct limine_hhdm_request hhdmReq = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

static void
getFbParams(struct fbParams *params)
{
    params->io = FRAMEBUFFER->address;
    params->width = FRAMEBUFFER->width;
    params->height = FRAMEBUFFER->height;
    params->pitch = FRAMEBUFFER->pitch;
    params->bpp = FRAMEBUFFER->bpp;
}

ULONG_PTR
keGetKernelBase(void)
{
    static struct limine_hhdm_response *hhdmResp = NULL;

    if (hhdmResp == NULL) {
        if ((hhdmResp = hhdmReq.response) == NULL) {
            keBugCheck("bootvars: could not get HHDM\n");
        }
    }

    if (hhmOffset == 0) {
        hhmOffset = hhdmResp->offset;
    }

    return hhmOffset;
}

int
keGetBootParams(struct bootParams *res, int flags)
{
    if (res == NULL) {
        return -1;
    }

    getFbParams(&res->fbParams);
    return 0;
}
