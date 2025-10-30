/*
 * ALGAE pointer box definitions
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _EX_BOX_H_
#define _EX_BOX_H_ 1

#include <ke/types.h>
#include <ntstatus.h>
#include <rtl/queue.h>

/*
 * Backend types for pointer boxes
 *
 * @PTRBOX_PAGES: Allocate pages
 * @PTRBOX_HEAP: Heap allocation
 */
typedef enum {
    PTRBOX_PAGES,
    PTRBOX_HEAP,
    PTRBOX_ANY
} PTRBOX_TYPE;

/*
 * Represents a single contigious region
 * of memory allocated by a backend
 */
typedef struct _PTRBOX_ELEMENT {
    ULONG magic;
    PTRBOX_TYPE type;
    union {
        void *ptr;
        ULONG_PTR address;
    };
    USIZE len;
    TAILQ_ENTRY(_PTRBOX_ELEMENT) link;
} PTRBOX_ELEMENT;

/*
 * A pointer box is a container for RAII style
 * pointer management
 *
 * @type: Type of backend
 * @ptrCount: Number of points in box
 * @entries: Entry queue
 */
typedef struct {
    USIZE ptrCount;
    TAILQ_HEAD(, _PTRBOX_ELEMENT) entries;
} PTRBOX;

/*
 * Initialize a pointer box
 *
 * @box: Box to initialize
 * @type: Type to initialize box with
 */
NTSTATUS exInitPtrBox(PTRBOX *box);

/*
 * Allocate memory of a specific type to a pointer box
 *
 * @box: Box to allocate to
 * @count: Number of bytes to allocate
 * @type: Type of memory to allocate
 * @result: Result
 */
NTSTATUS exBoxAllocate(
    PTRBOX *box, USIZE count,
    PTRBOX_TYPE type,
    void **result
);

/*
 * Release all memory of a specific type from a
 * pointerbox
 *
 * XXX: If 'type' is PTRBOX_ANY, all elements will be freed
 */
void exBoxRelease(PTRBOX *box, PTRBOX_TYPE type);

#endif  /* !_EX_BOX_H_ */
