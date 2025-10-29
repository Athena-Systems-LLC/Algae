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
 * Use exCreateProcess() instead of process
 * creation
 */
NTSTATUS keInitProcess(PROCESS *process);

#endif  /* !_KE_PROCESS_H_ */
