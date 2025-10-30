/*
 * ALGAE pointer box implentation
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ex/box.h>
#include <ex/pool.h>
#include <ex/trace.h>
#include <ke/defs.h>
#include <rtl/queue.h>
#include <mm/phys.h>
#include <mm/vm.h>

#define PTRBOX_MAGIC 0xCAFA17E

static void
boxFreeSingle(PTRBOX *box, PTRBOX_ELEMENT *elem)
{
    if (box == NULL || elem == NULL) {
        return;
    }

    /*
     * Remove it from the queue, but if the magic doesn't
     * match, log that there is potential corruption and
     * don't try to free anything but the descriptor.
     */
    TAILQ_REMOVE(&box->entries, elem, link);
    if (elem->magic != PTRBOX_MAGIC) {
        exTrace(EX_TRACE_ERR, "boxFreeSingle: possible ptrbox corruption\n");
        exFreePool(elem);
        return;
    }

    switch (elem->type) {
    case PTRBOX_PAGES:
        mmFreeFrame(elem->address, elem->len / PAGESIZE);
        break;
    case PTRBOX_HEAP:
        exFreePool(elem->ptr);
        break;
    }

    --box->ptrCount;
    exFreePool(elem);
}

NTSTATUS
exBoxAllocate(PTRBOX *box, USIZE count, PTRBOX_TYPE type, void **result)
{
    PTRBOX_ELEMENT *elem = NULL;

    if (box == NULL || result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (count == 0) {
        return STATUS_INVALID_PARAM;
    }

    elem = exAllocatePool(POOL_NON_PAGED, sizeof(*elem));
    if (elem == NULL) {
        return STATUS_NO_MEMORY;
    }

    /* Decide which kind of memory to allocate */
    switch (type) {
    case PTRBOX_PAGES:
        count = ALIGN_UP(count, PAGESIZE);
        elem->ptr = mmAllocPage(count / PAGESIZE, 0);
        if (elem->address == 0) {
            exFreePool(elem);
            return STATUS_NO_MEMORY;
        }

        elem->len = count;
        elem->type = type;
        break;
    case PTRBOX_HEAP:
        elem->ptr = exAllocatePool(POOL_NON_PAGED, count);
        if (elem->ptr == NULL) {
            exFreePool(elem);
            return STATUS_NO_MEMORY;
        }

        elem->len = count;
        elem->type = type;
        break;
    default:
        exFreePool(elem);
        return STATUS_INVALID_HANDLE;
    }

    elem->magic = PTRBOX_MAGIC;
    *(ULONG_PTR *)result = elem->address;
    TAILQ_INSERT_TAIL(&box->entries, elem, link);
    ++box->ptrCount;
    return STATUS_SUCCESS;
}

NTSTATUS
exInitPtrBox(PTRBOX *box)
{
    if (box == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    TAILQ_INIT(&box->entries);
    box->ptrCount = 0;
    return STATUS_SUCCESS;
}

void
exBoxRelease(PTRBOX *box, PTRBOX_TYPE type)
{
    PTRBOX_ELEMENT *elem;

    if (box == NULL) {
        return;
    }

    TAILQ_FOREACH(elem, &box->entries, link) {
        if (elem->type == type || type == PTRBOX_ANY) {
            boxFreeSingle(box, elem);
        }
    }
}
