/*
 * ALGAE interrupt request level management
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _KE_IRQL_H_
#define _KE_IRQL_H_ 1

#include <ke/types.h>
#include <hal/irql.h>

/*
 * Bring up the IRQL to the desired level
 *
 * @irql: Higher IRQL value to set
 *
 * Returns the old IRQL
 */
UCHAR keIrqlRaise(UCHAR irql);

/*
 * Drop to a lower IRQL
 *
 * @irql: Lower IRQl value to set
 *
 * Returns the old IRQL
 */
UCHAR keIrqlLower(UCHAR irql);

/*
 * Get the current IRQL level
 */
ULONG keGetIrql(void);

#endif  /* !_KE_IRQL_H_ */
