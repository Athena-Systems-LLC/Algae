/*
 * ALGAE Kernel entry.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/boot.h>
#include <ke/timer.h>
#include <ke/kpcr.h>
#include <ke/sched.h>
#include <md/mp.h>      /* shared */
#include <ex/pool.h>
#include <ob/objectStore.h>

static KPCR kpcrBsp;

void
kMain(void)
{
    kiPlatformInit();
    kiProcessorInit(&kpcrBsp);
    kiBootScreen();
    kiPoolInit();
    kiObjStoreInit();
    kiTimersInit();
    kiSchedInit();
    kiMpInit();
    for (;;);
}
