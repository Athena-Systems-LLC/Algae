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
 * @name: Name string (duplicated internally) [REQUIRED]
 * @type: Type of object to create            [REQUIRED]
 */
typedef struct {
    const CHAR *name;
    NT_OBJECT_TYPE type;
} NT_OBJECT_CREATE;

/*
 * An object is simply an opaque representation
 * of a system resource
 *
 * @type: Resource type (see NT_OB_*)
 * @name: Name of object
 * @data: Underlying object data
 */
typedef struct {
    NT_OBJECT_TYPE type;
    CHAR name[NT_OBNAME_LEN];
    void *data;
} NT_OBJECT;

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

#endif  /* !_OB_OBJECT_H_ */
