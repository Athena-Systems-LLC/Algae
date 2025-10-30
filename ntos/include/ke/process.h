/*
 * ALGAE process management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_PROCESS_H_
#define _KE_PROCESS_H_ 1

#include <ex/process.h>
#include <ntstatus.h>

/*
 * Initialize a process
 *
 * @process: Process to initialize
 * @flags: Optional flags
 *
 * Use exCreateProcess() instead of process
 * creation
 */
NTSTATUS keInitProcess(PROCESS *process, USHORT flags);

#endif  /* !_KE_PROCESS_H_ */
