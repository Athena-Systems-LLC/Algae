/*
 * ALGAE OMAR Boot Pack
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/bootPack.h>
#include <ke/defs.h>
#include <ke/types.h>
#include <ke/bootParam.h>
#include <ke/bugCheck.h>
#include <rtl/string.h>
#include <ntstatus.h>

#define OMAR_EOF "RAMO"
#define OMAR_REG    0
#define OMAR_DIR    1
#define BLOCK_SIZE 512

static char *bootPackBase = NULL;

/*
 * Represents the header of an OSMORA Archive Format
 * file
 *
 * @magic: Header magic ("OMAR")
 * @len: Length of the file
 * @nameLen: Length of the filename
 * @rev: OMAR revision
 * @mode: File permissions
 */
typedef struct PACKED {
    CHAR magic[4];
    UCHAR type;
    UCHAR nameLen;
    ULONG len;
    UCHAR rev;
    ULONG mode;
} OMAR_HDR;

/*
 * A single entry within a bootpack
 *
 * @path: Path of bootpack node
 * @length: Length of entry in bytes
 * @data: Data of entry
 */
typedef struct {
    const CHAR *path;
    ULONG length;
    UCHAR *data;
} BOOTPACK_NODE;

/*
 * Acquire a bootpack node from a path
 *
 * @path: Path to lookup
 * @result: Result of bootpack lookup
 */
static NTSTATUS
bootPackGetFile(const CHAR *path, BOOTPACK_NODE *result)
{
    BOOTPACK_NODE node;
    OMAR_HDR *hdr;
    const CHAR *p, *name;
    CHAR nameBuf[256];
    USIZE off;

    if (path == NULL || result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    p = bootPackBase;
    for (;;) {
        hdr = (OMAR_HDR *)p;
        if (rtlMemcmp(hdr->magic, OMAR_EOF, sizeof(OMAR_EOF)) == 0) {
            break;
        }

        /* Check the magic number */
        if (rtlMemcmp(hdr->magic, "OMAR", 4) != 0) {
            return STATUS_INVALID_PARAM;
        }

        if (hdr->nameLen > sizeof(nameBuf) - 1) {
            return STATUS_INVALID_PARAM;
        }

        name = (CHAR *)p + sizeof(OMAR_HDR);
        rtlMemcpy(nameBuf, name, hdr->nameLen);
        nameBuf[hdr->nameLen] = '\0';

        /* Compute offset to next block */
        if (hdr->type == OMAR_DIR) {
            off = 512;
        } else {
            off = ALIGN_UP(sizeof(*hdr) + hdr->nameLen + hdr->len, BLOCK_SIZE);
        }


        /* Skip header and name to the data */
        p = (CHAR *)hdr + sizeof(OMAR_HDR);
        p += hdr->nameLen;

        if (rtlStrcmp(nameBuf, path) == 0) {
            node.length = hdr->len;
            node.data = (void *)p;
            *result = node;
            return STATUS_SUCCESS;
        }

        hdr = (OMAR_HDR *)((CHAR *)hdr + off);
        p = (CHAR *)hdr;
        rtlMemset(nameBuf, 0, sizeof(nameBuf));
    }
    return STATUS_FILE_UNAVL;
}

CHAR *
keBootPackGet(const CHAR *path)
{
    BOOTPACK_NODE node;
    NTSTATUS status;

    if (path == NULL) {
        return NULL;
    }

    if (*path != '/') {
        return NULL;
    }

    ++path;
    status = bootPackGetFile(path, &node);
    if (status != STATUS_SUCCESS) {
        return NULL;
    }

    return node.data;
}

void
kiBootPackInit(void)
{
    if (bootPackBase != NULL) {
        return;
    }

    bootPackBase = keGetBootModule("/boot/bootpack.omar");
    if (bootPackBase == NULL) {
        keBugCheck("failed to get bootpack base\n");
    }
}
