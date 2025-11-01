/*
 * ALGAE Module export layer
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <ke/module.h>
#include <ke/types.h>

/* From linker script */
extern CHAR __modules_init_start[];
extern CHAR __modules_init_end[];

void
kiModuleInit(MODULE_TYPE type)
{
    KERNEL_MODULE *modp = (KERNEL_MODULE *)__modules_init_start;

    /* Go through each module and initialize them */
    while (modp < (KERNEL_MODULE *)__modules_init_end) {
        /* Does it match the requested type? */
        if (modp->type != type) {
            ++modp;
            continue;
        }

        if (modp->init != NULL) {
            modp->init();
        }

        ++modp;
    }
}
