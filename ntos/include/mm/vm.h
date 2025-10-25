/*
 * ALGAE Virtual Memory Management interface
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#ifndef _MM_VM_H_
#define _MM_VM_H_ 1

#include <ke/bootParam.h>

#define PAGESIZE 4096
#define PHYS_TO_VIRT(PA) (void *)((PA) + keGetKernelBase())
#define VIRT_TO_PHYS(VIRT) (((ULONG_PTR)(VIRT)) - keGetKernelBase())

#endif  /* !_MM_VM_H_ */
