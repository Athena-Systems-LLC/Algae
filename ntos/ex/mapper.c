/*
 * ALGAE executive mapper layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ex/mapper.h>
#include <ob/object.h>
#include <ke/bugCheck.h>
#include <ke/defs.h>
#include <ke/module.h>
#include <mm/vm.h>

static NT_OBJECT *mapper;

/*
 * Create a fixed mapping from a region descriptor
 * of a mapper object
 *
 * @region: Fixed region to map
 */
static NTSTATUS
mapperMapFixed(MAPPER_REGION *region)
{
    void *retVal;
    MM_REGION mmRegion;
    MMU_VAS vas;

    vas = mmGetCurrentVas();
    mmRegion.pBase = region->pBase;
    mmRegion.vBase = (void *)region->vBase;
    mmRegion.byteCount = region->length;
    retVal = mmMapPages(
        &vas,
        &mmRegion,
        region->prot,
        0
    );

    return (retVal == NULL) ? STATUS_NO_MEMORY : STATUS_SUCCESS;
}


NTSTATUS
exMapperGet(const CHAR *path, MAPPER_OBJECT **result)
{
    NT_OBJECT *object;
    NTSTATUS status;

    if (path == NULL || result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    status = obLookupObject(path, &object, 0);
    if (status != STATUS_SUCCESS) {
        return status;
    }

    /* This needs to be a mapper descriptor */
    if (object->type != NT_OB_MAPPER) {
        return STATUS_BAD_OBJ_TYPE;
    }

    /* We need data to return */
    if (object->data == NULL) {
        return STATUS_NOT_SUPPORTED;
    }

    *result = object->data;
    return STATUS_SUCCESS;
}

NTSTATUS
exMapperMap(MAPPER_OBJECT *mapper)
{
    NTSTATUS status = STATUS_NOT_SUPPORTED;
    MAPPER_REGION *region;

    if (mapper == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    switch (mapper->type) {
    case MAPPER_FIXED:
        status = mapperMapFixed(&mapper->region);
        break;
    case MAPPER_DYNAMIC:
        /* XXX: Support this */
        break;
    }

    return status;
}

/*
 * Initialize the mapper directory
 */
static NTSTATUS
mapperInit(void)
{
    NTSTATUS status;
    NT_OBJECT_CREATE params = {
        .parent = "/",
        .name = "mapper",
        .type = NT_OB_DIRECTORY
    };

    /* We require this to work */
    status = obCreateObject(&params, &mapper);
    if (status != STATUS_SUCCESS) {
        keBugCheck("could not initialize mapper\n");
    }

    status = obAllocateDir(mapper);
    if (status != STATUS_SUCCESS) {
        keBugCheck("could not allocate mapper directory\n");
    }

    return STATUS_SUCCESS;
}

MODULE_EXPORT("mapper", mapperInit, MODULE_OBJECT);
