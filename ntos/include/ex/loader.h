/*
 * ALGAE Program Loader definitions.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _EX_LOADER_H_
#define _EX_LOADER_H_

#include <ke/types.h>
#include <ntstatus.h>

/*
 * Represents a loaded program from the
 * loader
 *
 * @entry: Entrypoint of program
 */
typedef struct {
    ULONG_PTR entry;
} LOADER_PROGRAM;

/*
 * Load a program from the bootpack
 *
 * @path: Path of program to load
 * @result: Result is written here
 */
NTSTATUS exLoadFromBootPack(
    const CHAR *path,
    LOADER_PROGRAM *result
);

#endif  /* !_EX_LOADER_H_ */
