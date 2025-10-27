/*
 * ALGAE NT status codes
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _SDK_NTSTATUS_H_
#define _SDK_NTSTATUS_H_ 1

#include <ke/types.h>

typedef ULONG NTSTATUS;

#define STATUS_SUCCESS          0x00000000  /* Success */
#define STATUS_ACCESS_DENIED    0xC0000022  /* Access denied */
#define STATUS_NO_MEMORY        0xC0000017  /* Out of memory */
#define STATUS_BAD_OBJNAME      0xC0000033  /* Object name invalid */
#define STATUS_INVALID_HANDLE   0xC0000008  /* Invalid handle */
#define STATUS_BAD_OBJ_TYPE     0xC0000024  /* Bad object type */
#define STATUS_OBJECT_NOTFOUND  0xC0000034  /* Object name not found */
#define STATUS_NOT_DIRECTORY    0xC0000039  /* Not a directory */
#define STATUS_FILE_UNAVL       0xC0000467  /* File unavailable */
#define STATUS_NOT_MAPPED       0xC0000019  /* View not mapped */

#endif  /* !_SDK_NTSTATUS_H_ */
