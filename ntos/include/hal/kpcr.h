/*
 * ALGAE KPCR HAL layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_KPCR_H_
#define _HAL_KPCR_H_

#include <ke/types.h>
#include <ntstatus.h>

struct kpcr;

/*
 * Get a processor by index
 *
 * @cpuIdx: Index of processor to get
 * @result: Result is written here
 */
NTSTATUS halCpuGet(USHORT cpuIdx, struct kpcr **result);

/*
 * Returns the number of processors on the system
 */
USIZE halCpuCount(void);

#endif
