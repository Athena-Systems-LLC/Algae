/*
 * ALGAE Local APIC interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MACHINE_LAPIC_H_
#define _MACHINE_LAPIC_H_ 1

#include <ke/types.h>
#include <ntstatus.h>

/*
 * Destination shorthand
 *
 * @IPI_SHAND_NONE: No shorthand
 * @IPI_SHAND_SELF: Send IPI to self
 * @IPI_SHAND_AIS: Send IPI to all including self
 * @IPI_SHAND_AXS: Send IPI to all excluding self
 */
typedef enum {
    IPI_SHAND_NONE,
    IPI_SHAND_SELF,
    IPI_SHAND_AIS,
    IPI_SHAND_AXS
} IPI_SHORTHAND;

/* Delivery mode */
#define IPI_DELMOD_FIXED    0x0     /* Fixed */
#define IPI_DELMOD_LOWPRI   0x1     /* Lowest priority */
#define IPI_DELMOD_INIT     0x5     /* Init (for bootstrap) */
#define IPI_DELMOD_STARTUP  0x6     /* Startup (for bootstrap) */

/* See above */
typedef UCHAR IPI_DELMOD;

/* Destination mode */
#define IPI_DESTMODE_PHYSICAL 0x0
#define IPI_DESTMODE_LOGICAL  0x01

/*
 * Local APIC interprocessor interrupt
 *
 * @delmod: Delivery mode
 * @shand: Shorthand
 * @vector: Interrupt vector
 * @apicId: ID of target APIC
 * @destMode: Destination mode
 */
typedef struct {
    IPI_DELMOD delmod;
    IPI_SHORTHAND shand;
    UCHAR vector;
    UCHAR apicId;
    UCHAR destMode;
} LAPIC_IPI;

/*
 * Send an interprocessor interrupt from one Local APIC
 * to another Local APIC on the machine
 */
NTSTATUS kiLapicSendIpi(const LAPIC_IPI *ipi);

/*
 * Internal routine used to initialize the
 * Local APIC on the current processor core
 */
void kiLapicInit(void);

#endif /* !_MACHINE_LAPIC_H_ */
