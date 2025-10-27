/*
 * ALGAE object manager object definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _OB_OBJECT_H_
#define _OB_OBJECT_H_ 1

#include <ntstatus.h>
#include <ke/types.h>
#include <rtl/queue.h>

#define NT_OBNAME_LEN 32    /* Max length of object name */

/*
 * Valid object types
 *
 * XXX: __NT_OB_MAX is used internally for counting how many
 *      object types there are.
 */
typedef enum {
    NT_OB_DIRECTORY,
    __NT_OB_MAX,
} NT_OBJECT_TYPE;

/*
 * Arguments for creating objects are stored
 * in here
 *
 * @parent: Path of parent                    [NULL if unused]
 * @name: Name string (duplicated internally) [REQUIRED]
 * @type: Type of object to create            [REQUIRED]
 */
typedef struct {
    const CHAR *parent;
    const CHAR *name;
    NT_OBJECT_TYPE type;
} NT_OBJECT_CREATE;

/*
 * An object is simply an opaque representation
 * of a system resource
 *
 * @type: Resource type (see NT_OB_*)
 * @name: Name of object
 * @ref: Reference count
 * @data: Underlying object data
 * @link: Used for object directories
 */
typedef struct nt_obj {
    NT_OBJECT_TYPE type;
    CHAR name[NT_OBNAME_LEN];
    USHORT ref;
    void *data;
    TAILQ_ENTRY(nt_obj) link;
} NT_OBJECT;

/*
 * Represents an object directory that stores a list
 * of objects
 *
 * @dir: Directory of objects
 * @numEnt: Number of objects in this directory
 */
typedef struct {
    TAILQ_HEAD(, nt_obj) dir;
    USIZE numEnt;
} NT_OBJECTDIR;

/*
 * Create an object with specific parameters
 *
 * @params: Arguments for object creation
 * @result: Resulting object pointer is written here
 */
NTSTATUS obCreateObject(
    NT_OBJECT_CREATE *params,
    NT_OBJECT **result
);

/*
 * Release an object from memory
 *
 * @object: Object to free from memory
 */
NTSTATUS obFreeObject(NT_OBJECT *object);

/*
 * Lookup an object from the object
 * namespace
 *
 * @path: Path of object
 * @result: Result is written here
 * @flags: Optional flags
 */
NTSTATUS obLookupObject(
    const CHAR *path, NT_OBJECT **result,
    USHORT flags
);

/*
 * Allocate an object directory to an object that
 * is marked of type directory.
 *
 * When a directory object is created, its data field
 * still needs to be initialized.
 *
 * @dirObj: Directory object to allocate for
 */
NTSTATUS obAllocateDir(
    NT_OBJECT *dirObj
);

#endif  /* !_OB_OBJECT_H_ */
