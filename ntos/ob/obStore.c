/*
 * ALGAE object storage manager core
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ob/object.h>
#include <ex/pool.h>
#include <rtl/string.h>
#include <ntstatus.h>

NTSTATUS
obCreateObject(NT_OBJECT_CREATE *params, NT_OBJECT **result)
{
    NT_OBJECT *object;
    USIZE nameLen;

    if (params == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (params->type >= __NT_OB_MAX) {
        return STATUS_BAD_OBJ_TYPE;
    }

    /* Name cannot be NULL */
    if (params->name == NULL) {
        return STATUS_BAD_OBJNAME;
    }

    nameLen = rtlStrlen(params->name);
    if (nameLen == 0 || nameLen >= NT_OBNAME_LEN - 1) {
        return STATUS_BAD_OBJNAME;
    }

    /* Allocate the actual object now */
    object = exAllocatePool(POOL_NON_PAGED, sizeof(*object));
    if (object == NULL) {
        return STATUS_NO_MEMORY;
    }

    rtlMemset(object, 0, sizeof(*object));
    object->type = params->type;
    rtlMemcpy(object->name, params->name, nameLen);

    *result = object;
    return STATUS_SUCCESS;
}

NTSTATUS
obFreeObject(NT_OBJECT *object)
{
    if (object == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    exFreePool(object);
    return STATUS_SUCCESS;
}
