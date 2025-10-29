/*
 * ALGAE Kernel Processor Control Region
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _HAL_KPCR_H_
#define _HAL_KPCR_H_ 1

#include <ke/types.h>
#include <ke/sched.h>
#include <ke/defs.h>
#include <ke/spinlock.h>
#include <hal/kpcr.h>
#include <md/mcb.h>     /* standard */
#include <ntstatus.h>

/*
 * Possible ways kernel processor arbitration can
 * can acomplished, in other words, the various possible
 * modes the arbiter can be in.
 */
typedef enum {
    KPARB_ROUND_ROBIN
} KPARB_METHOD;

/*
 * The kernel processor arbiter decides which
 * processors are to be selected next for a
 * particular operation e.g., process scheduling
 *
 * @method: Arbitration mode
 * @semaphore: Used for tracking cores
 * @lock: Protects the fields of this structure
 */
typedef struct {
    KPARB_METHOD method;
    USHORT semaphore;
    KSPIN_LOCK lock;
} KPARB;

/*
 * The kernel processor control region is standard between
 * architectures and contains general MI information about
 * a processor core
 *
 * @vId: Virtual / logical ID (assigned by the kernel)
 * @core: MD information
 */
typedef struct kpcr {
    USHORT vId;
    MCB core;
    struct kpcr *self;
} KPCR;

/*
 * Get the KPCR for the currently executing
 * processor core
 *
 * XXX: This counts as HAL and also counts as KE as
 *      its a really general function.
 */
KPCR *keGetCore(void);

/*
 * Get a KPCR by index
 *
 * @index: Index to select
 */
KPCR *keCpuGet(USIZE index);

/*
 * Get the number of processors on the
 * system
 */
USIZE keCpuCount(void);

/*
 * Initialize a processor arbiter
 *
 * @mode: Mode to initialize in
 * @result: Result is written here
 */
NTSTATUS keParbReset(KPARB_METHOD mode, KPARB *result);

/*
 * Cycles the arbiter depending on its mode
 *
 * @arbiter: Arbiter to cycle
 *
 * Returns the current processor selected
 */
KPCR *keParbCycle(KPARB *arbiter);

#endif  /* !_HAL_KPCR_H_ */
