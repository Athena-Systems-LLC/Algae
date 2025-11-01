/*
 * ALGAE Module export layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_MODULE_H_
#define _KE_MODULE_H_ 1

#include <ke/types.h>
#include <ke/defs.h>
#include <ntstatus.h>

/*
 * Valid module types
 *
 * @MODULE_GENERIC: Use for generic subsystems like drivers
 * @MODULE_OBJECT: Used for object initialization
 */
typedef enum {
    MODULE_GENERIC,
    MODULE_OBJECT
} MODULE_TYPE;

/*
 * Represents a system kernel module
 *
 * @name: Name of the module
 * @init: Initialization function
 */
typedef struct {
    CHAR *name;
    NTSTATUS(*init)(void);
    MODULE_TYPE type;
} KERNEL_MODULE;

/*
 * Export a module so that it is initialized on
 * startup
 */
#define MODULE_EXPORT(NAME, INIT_FN, TYPE)      \
    USED SECTION(".modules")                    \
    static KERNEL_MODULE __module = {           \
        .name = (NAME),                         \
        .init = (INIT_FN),                      \
        .type = (TYPE)                          \
    }

/*
 * Internal routine used to initialize modules of a
 * specific type
 *
 * @type: Type of module to initialize
 */
void kiModuleInit(MODULE_TYPE type);

#endif  /* !_KE_MODULE_H_ */
