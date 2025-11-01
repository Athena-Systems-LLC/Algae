/*
 * ALGAE object table management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ob/table.h>
#include <ob/object.h>
#include <rtl/string.h>
#include <ex/pool.h>
#include <ntstatus.h>

NTSTATUS
obInitTable(NT_OBTAB *table)
{
    if (table == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    table->openCount = 0;
    rtlMemset(table->ob, 0, sizeof(table->ob));
    return STATUS_SUCCESS;
}

NTSTATUS
obTableAdd(NT_OBTAB *table, NT_OB_HANDLE *handRes, NT_OBJECT *ob)
{
    NTSTATUS status = STATUS_SUCCESS;
    NT_OB_HANDLE handle = -1;

    if (table == NULL || handRes == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (ob == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    keAcquireSpinLock(&table->lock);
    for (NT_OB_HANDLE i = 0; i < NT_OBTAB_ENTRIES; ++i) {
        if (table->ob[i] != NULL) {
            continue;
        }

        /* Add the object and bump its ref count */
        table->ob[i] = ob;
        handle = i;
        ++ob->ref;
        break;
    }

    keReleaseSpinLock(&table->lock);
    if (handle < 0) {
        return STATUS_NO_SPACE;
    }

    *handRes = handle;
    return status;
}
