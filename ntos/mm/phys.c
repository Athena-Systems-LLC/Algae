/*
 * ALGAE Physical Memory Management core
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <mm/phys.h>
#include <mm/vm.h>
#include <ke/bugCheck.h>
#include <ke/defs.h>
#include <ke/spinlock.h>
#include <rtl/string.h>
#include <rtl/limine.h>

static KSPIN_LOCK lock;
static USIZE highestFrameIdx;
static USIZE bitmapSize;
static USIZE nFramesFree;
static USIZE framesUsed = 0;
static USIZE framesFree = 0;
static USIZE bitmapFreeStart;
static USIZE bitmapSize;
static USIZE lastIdx = 0;
static UCHAR *bitmap;

static struct limine_memmap_response *resp = NULL;
static struct limine_memmap_request mmapReq = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static void
mmPopulateBitmap(void)
{
    struct limine_memmap_entry *memEntry;

    for (USIZE i = 0; i < resp->entry_count; ++i) {
        memEntry = resp->entries[i];

        if (memEntry->type != LIMINE_MEMMAP_USABLE) {
            framesUsed += memEntry->length / PAGESIZE;
            continue;
        }

        if (bitmapFreeStart == 0) {
            bitmapFreeStart = memEntry->base / PAGESIZE;
        }

        for (USIZE j = 0; j < memEntry->length; j += PAGESIZE) {
            clrBit(bitmap, (memEntry->base + j) / PAGESIZE);
        }

        framesFree += memEntry->length / PAGESIZE;
    }
}

static void
mmAllocBitmap(void)
{
    struct limine_memmap_entry *memEntry;

    for (USIZE i = 0; i < resp->entry_count; ++i) {
        memEntry = resp->entries[i];

        if (memEntry->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        if (memEntry->length >= bitmapSize) {
            bitmap = PHYS_TO_VIRT(memEntry->base);
            rtlMemset(bitmap, 0xFF, bitmapSize);
            memEntry->length -= bitmapSize;
            memEntry->base += bitmapSize;
            return;
        }
    }
}

static void
mmInitBitmap(void)
{
    struct limine_memmap_entry *memEntry;
    ULONG_PTR highestAddr;

    for (USIZE i = 0; i < resp->entry_count; ++i) {
        memEntry = resp->entries[i];
        if (memEntry->type != LIMINE_MEMMAP_USABLE) {
            continue;
        }

        highestAddr = MAX(
            highestAddr,
            memEntry->base + memEntry->length
        );
    }

    highestFrameIdx = highestAddr / PAGESIZE;
    bitmapSize = ALIGN_UP(highestFrameIdx / 8, PAGESIZE);

    mmAllocBitmap();
    mmPopulateBitmap();
}

static ULONG_PTR
mmDoAllocFrames(USIZE count)
{
    LONG nFrames = 0;
    QUAD idx = -1;
    ULONG_PTR ret = 0;

    for (USIZE i = 0; i < highestFrameIdx; ++i) {
        if (!testBit(bitmap, i)) {
            if (idx < 0)
                idx = i;
            if (nFrames++ >= count)
                break;

            continue;
        }

        idx = -1;
        nFrames = 0;
    }

    if (idx < 0 || (nFrames - 1) < count) {
        return 0;
    }

    for (USIZE i = idx; i < idx + count; ++i) {
        setBit(bitmap, i);
    }

    ret = idx * PAGESIZE;
    lastIdx = idx;
    rtlMemset(PHYS_TO_VIRT(ret), 0, count * PAGESIZE);
    return ret;
}

ULONG_PTR
mmAllocFrame(USIZE count)
{
    ULONG_PTR ret;

    keAcquireSpinLock(&lock);
    framesUsed += count;
    framesFree -= count;
    if ((ret = mmDoAllocFrames(count)) == 0) {
        lastIdx = 0;
        ret = mmDoAllocFrames(count);
    }

    keReleaseSpinLock(&lock);
    return ret;
}

void
mmFreeFrame(ULONG_PTR base, USIZE count)
{
    USIZE p, stop_at;

    keAcquireSpinLock(&lock);
    p = ALIGN_UP(base, PAGESIZE);
    stop_at = p + (count * PAGESIZE);

    while (p < stop_at) {
        clrBit(bitmap, p / PAGESIZE);
        p += PAGESIZE;
    }

    keReleaseSpinLock(&lock);
}

void
mmPhysInit(void)
{
    resp = mmapReq.response;
    mmInitBitmap();
}
