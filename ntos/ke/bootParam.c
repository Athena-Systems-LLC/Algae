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
#include <rtl/string.h>

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
static volatile struct limine_rsdp_request rsdpReq = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};
static volatile struct limine_module_request modReq = {
    .id = LIMINE_MODULE_REQUEST,
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

char *
keGetBootModule(const char *path)
{
    struct limine_module_response *modResp;
    struct limine_file *file;
    USIZE modCount;

    modResp = modReq.response;
    if (modResp == NULL) {
        keBugCheck("unable to get the boot module\n");
    }

    modCount = modResp->module_count;
    for (USIZE i = 0; i < modCount; ++i) {
        file = modResp->modules[i];
        if (rtlStrcmp(file->path, path) == 0) {
            return file->address;
        }
    }

    return NULL;
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
    struct limine_rsdp_response *rsdpResp;

    if (res == NULL) {
        return -1;
    }

    getFbParams(&res->fbParams);
    /* We need this for proper operation */
    if ((rsdpResp = rsdpReq.response) == NULL) {
        keBugCheck("bootvars: could not get ACPI RSDP\n");
    }
    res->acpiRsdp = rsdpResp->address;
    return 0;
}
