/*
 * ALGAE AMD64 GDT implementation.
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <md/gdt.h>

struct gdtEntry g_gdtData[GDT_ENTRY_COUNT] = {
    /* Null */
    {0},

    /* Kernel code (0x08) */
    {
        .limit      = 0x0000,
        .baseLow    = 0x0000,
        .baseMid    = 0x00,
        .attributes = GDT_ATTRIBUTE_64BIT_CODE | GDT_ATTRIBUTE_PRESENT   |
                      GDT_ATTRIBUTE_DPL0       | GDT_ATTRIBUTE_NONSYSTEM |
                      GDT_ATTRIBUTE_EXECUTABLE | GDT_ATTRIBUTE_READABLE,
        .baseHi     = 0x00
    },

    /* Kernel data (0x10) */
    {
        .limit      = 0x0000,
        .baseLow    = 0x0000,
        .baseMid    = 0x00,
        .attributes = GDT_ATTRIBUTE_PRESENT    | GDT_ATTRIBUTE_DPL0      |
                      GDT_ATTRIBUTE_NONSYSTEM  | GDT_ATTRIBUTE_WRITABLE,
        .baseHi     = 0x00
    },

    /* User code (0x18) */
    {
        .limit      = 0x0000,
        .baseLow    = 0x0000,
        .baseMid    = 0x00,
        .attributes = GDT_ATTRIBUTE_64BIT_CODE | GDT_ATTRIBUTE_PRESENT   |
                      GDT_ATTRIBUTE_DPL3       | GDT_ATTRIBUTE_NONSYSTEM |
                      GDT_ATTRIBUTE_EXECUTABLE | GDT_ATTRIBUTE_READABLE,
        .baseHi     = 0x00
    },

    /* User data (0x20) */
    {
        .limit      = 0x0000,
        .baseLow    = 0x0000,
        .baseMid    = 0x00,
        .attributes = GDT_ATTRIBUTE_PRESENT    | GDT_ATTRIBUTE_DPL3      |
                      GDT_ATTRIBUTE_NONSYSTEM  | GDT_ATTRIBUTE_WRITABLE,
        .baseHi     = 0x00
    },

    /*
     * TSS segment (0x28)
     *
     * NOTE: 64-bit TSS descriptors are 16 bytes, equivalent to the size of two
     *       regular descriptor entries.
     *       See Intel SPG 3/25 Section 9.2.3 - TSS Descriptor in 64-bit mode.
     */
    {0}, {0}
};
