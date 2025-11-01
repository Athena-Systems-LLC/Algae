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
#include <ke/bugCheck.h>
#include <ke/bootPack.h>
#include <ke/module.h>
#include <ke/loader.h>
#include <hal/mmu.h>
#include <hal/process.h>
#include <md/mp.h>      /* shared */
#include <ke/loader.h>
#include <ex/process.h>
#include <ex/pool.h>
#include <ob/objectStore.h>

static KPCR kpcrBsp;

void
kMain(void)
{
    PROCESS *proc;
    NTSTATUS status;
    LOADER_PROGRAM prog;
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

    /* Initialize modules */
    kiModuleInit(MODULE_OBJECT);
    kiModuleInit(MODULE_GENERIC);

    status = exCreateProcess(PROCESS_UNAMED, &proc, 0);
    if (status != STATUS_SUCCESS) {
        keBugCheck("failed to create startup process\n");
    }

    /* Load the first process */
    mmuWriteVas(&proc->pcb.vas, 0);
    status = keLoadFromBootPack("/system64/csrs.exe", &prog);
    if (status != STATUS_SUCCESS) {
        keBugCheck("failed to load csrs.exe from system64\n");
    }

    /* Start it up and kick it! */
    halProcSetIp(&proc->pcb, prog.entry);
    halProcKick(&proc->pcb);
}
