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
#include <ke/bootPack.h>
#include <md/mp.h>      /* shared */
#include <ke/loader.h>
#include <ex/pool.h>
#include <ob/objectStore.h>

static KPCR kpcrBsp;

void
kMain(void)
{
    NTSTATUS status;
    MMU_VAS vas;

    kiPlatformInit();
    kiProcessorInit(&kpcrBsp);
    kiBootScreen();
    kiPoolInit();
    kiObjStoreInit();
    kiTimersInit();
    kiSchedInit();
    kiMpInit();
    kiBootPackInit();

    status = mmuCreateVas(&vas);
    mmuWriteVas(&vas, 0);
    if (status != STATUS_SUCCESS) {
        keBugCheck("failed to create new VAS\n");
    }

    for (;;);
}
