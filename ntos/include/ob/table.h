/*
 * ALGAE object table management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _OB_TABLE_H_
#define _OB_TABLE_H_ 1

#include <ke/types.h>
#include <ke/spinlock.h>
#include <ob/object.h>
#include <ntstatus.h>

/*
 * Represents an object handle that can be
 * used to represent an opened object, values
 * less than zero are invalid
 */
typedef SHORT NT_OB_HANDLE;

/*
 * Represents an object table used per processes
 * to hold references to opened objects
 *
 * @ob: The actual object table
 * @openCount: The number of open objects
 * @lock: Protects the structure
 */
typedef struct {
    NT_OBJECT *ob[NT_OBTAB_ENTRIES];
    USHORT openCount;
    KSPIN_LOCK lock;
} NT_OBTAB;

/*
 * Initialize an object table
 *
 * @table: Table to initialize
 */
NTSTATUS obInitTable(NT_OBTAB *table);

/*
 * Add an object to an object table
 *
 * @table: Table to add to
 * @handleResult: Handle result is written here
 * @object: Object to be added to table
 */
NTSTATUS obTableAdd(
    NT_OBTAB *table,
    NT_OB_HANDLE *handleResult,
    NT_OBJECT *object
);

#endif  /* !_OB_TABLE_H_ */
