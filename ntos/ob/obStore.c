/*
 * ALGAE object storage manager core
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ob/object.h>
#include <ke/bugCheck.h>
#include <ex/pool.h>
#include <ex/trace.h>
#include <rtl/string.h>
#include <ntstatus.h>

/*
 * The root object is always a directory type and stores
 * a list of object subdirectories and/or objects themselves
 */
static NT_OBJECT *rootObject = NULL;

/*
 * Add a child object to a parent object
 */
static NTSTATUS
obDirectoryAdd(NT_OBJECT *parent, NT_OBJECT *child)
{
    NT_OBJECTDIR *dir;

    if (parent == NULL || child == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (parent->type != NT_OB_DIRECTORY) {
        return STATUS_NOT_DIRECTORY;
    }

    if ((dir = parent->data) == NULL) {
        return STATUS_OBJECT_NOTFOUND;
    }

    TAILQ_INSERT_TAIL(&dir->dir, child, link);
    ++dir->numEnt;
    return STATUS_SUCCESS;
}

/*
 * Lookup a specific name within an object directory
 *
 * @parent: Object to lookup within
 * @name: Name to lookup
 * @result: Result pointer is written here if found
 */
static NTSTATUS
obDirectoryLookup(NT_OBJECT *parent, const CHAR *name, NT_OBJECT **result)
{
    NT_OBJECTDIR *dir;
    NT_OBJECT *cur;

    if (parent == NULL || name == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    if (result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    /* Must be a directory object */
    if (parent->type != NT_OB_DIRECTORY) {
        return STATUS_BAD_OBJ_TYPE;
    }

    if ((dir = parent->data) == NULL) {
        return STATUS_OBJECT_NOTFOUND;
    }

    TAILQ_FOREACH(cur, &dir->dir, link) {
        if (rtlStrcmp(cur->name, name) == 0) {
            *result = cur;
            return STATUS_SUCCESS;
        }
    }

    return STATUS_OBJECT_NOTFOUND;
}

NTSTATUS
obCreateObject(NT_OBJECT_CREATE *params, NT_OBJECT **result)
{
    NT_OBJECT *object, *parent = NULL;
    NTSTATUS status;
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

    /* Is there a parent we want to add to? */
    if (params->parent != NULL) {
        status = obLookupObject(params->parent, &parent, 0);
        if (status != STATUS_SUCCESS)
            return STATUS_BAD_OBJNAME;
        if (parent->type != NT_OB_DIRECTORY)
            return STATUS_NOT_DIRECTORY;
    }

    /* Ensure the name length is correct */
    nameLen = rtlStrlen(params->name);
    if (nameLen == 0 || nameLen >= NT_OBNAME_LEN - 1) {
        return STATUS_BAD_OBJNAME;
    }

    /* Allocate the actual object now */
    object = exAllocatePool(POOL_NON_PAGED, sizeof(*object));
    if (object == NULL) {
        return STATUS_NO_MEMORY;
    }

    /* Initialize the object */
    rtlMemset(object, 0, sizeof(*object));
    object->type = params->type;
    object->ref = 1;
    rtlMemcpy(object->name, params->name, nameLen);

    /*
     * If there is a parent we should add this object
     * to, add it and bump the ref
     */
    if (parent != NULL) {
        ++object->ref;
        obDirectoryAdd(parent, object);
    }

    *result = object;
    return STATUS_SUCCESS;
}

NTSTATUS
obFreeObject(NT_OBJECT *object)
{
    if (object == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    /*
     * Only free the memory if there are zero
     * outstanding references to the object
     */
    if ((--object->ref) == 0) {
        exFreePool(object);
    }

    return STATUS_SUCCESS;
}

void
kiObjStoreInit(void)
{
    NT_OBJECT_CREATE create;
    NT_OBJECTDIR *dir;
    NTSTATUS status;

    create.parent = NULL;
    create.name = "/";
    create.type = NT_OB_DIRECTORY;

    status = obCreateObject(&create, &rootObject);
    if (status != STATUS_SUCCESS) {
        keBugCheck("kiObjStoreInit: failed to init object broker\n");
    }

    rootObject->data = exAllocatePool(POOL_NON_PAGED, sizeof(NT_OBJECTDIR));
    if (rootObject->data == NULL) {
        keBugCheck("kiObjStoreInit: failed to init root object\n");
    }

    dir = rootObject->data;
    TAILQ_INIT(&dir->dir);
    dir->numEnt = 0;
}

NTSTATUS
obLookupObject(const CHAR *path, NT_OBJECT **result, USHORT flags)
{
    const CHAR *tail;
    CHAR component[NT_OBNAME_LEN];
    NT_OBJECT *curObject = rootObject;
    NTSTATUS status;
    USIZE i = 0;

    if (path == NULL || result == NULL) {
        return STATUS_INVALID_HANDLE;
    }

    tail = path;

    /* Skip any leading slashes */
    while (*tail == '/' && *tail != '\0') {
        ++tail;
    }

    /*
     * Assume root was requested if this is the
     * end of the path
     */
    if (*tail == '\0') {
        *result = rootObject;
        return STATUS_SUCCESS;
    }

    /* Begin parsing each component */
    while (*tail != '\0') {
        while (*tail != '/' && *tail != '\0') {
            component[i++] = *tail++;

            /* Don't overflow */
            if (i >= NT_OBNAME_LEN - 1)
                return STATUS_BAD_OBJNAME;
        }

        component[i] = '\0';
        status = obDirectoryLookup(
            curObject,
            component,
            &curObject
        );

        if (status != STATUS_SUCCESS) {
            return status;
        }

        /* Reset counters and component buffer */
        rtlMemset(component, 0, sizeof(component));
        i = 0;

        /* Skip the delimiter */
        while (*tail == '/' && *tail != '\0') {
            ++tail;
        }
    }

    *result = curObject;
    return STATUS_SUCCESS;
}
