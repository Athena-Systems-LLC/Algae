/*
 * ALGAE common ACPI API
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _DRV_ACPI_H_
#define _DRV_ACPI_H_

#include <ke/types.h>
#include <acpi/tables.h>

/*
 * Initialize the ACPI subsystem
 */
void acpiInit(void);

/*
 * Lookup a table and return the virtual base address
 * on success, otherwise NULL if not found
 */
void *acpiQueryTable(const char *signature);

/*
 * Read an ACPI MADT entry of type 'type' and invoke
 * callback 'cb'
 */
int acpiReadMadt(ULONG type, int(*cb)(APIC_HEADER *, USIZE arg), USIZE arg);

#endif  /* !_DRV_ACPI_H_ */
