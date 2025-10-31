/*
 * ALGAE Program Loader definitions.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_LOADER_H_
#define _KE_LOADER_H_

#include <ke/types.h>
#include <ntstatus.h>
#include <ex/box.h>

/*
 * Represents a loaded program from the
 * loader
 *
 * @entry: Entrypoint of program
 */
typedef struct {
    ULONG_PTR entry;
    PTRBOX runTimeInfo;
} LOADER_PROGRAM;

/*
 * Load a program from the bootpack
 *
 * XXX: This function maps the currently loaded
 *      virtual address space
 *
 * @path: Path of program to load
 * @result: Result is written here
 */
NTSTATUS keLoadFromBootPack(
    const CHAR *path,
    LOADER_PROGRAM *result
);

#endif  /* !_KE_LOADER_H_ */
