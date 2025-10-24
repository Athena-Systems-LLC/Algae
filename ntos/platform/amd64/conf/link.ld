/*
 * Algae linker script for AMD64 port
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

ENTRY(_start)

SECTIONS {
    . = 1M;

    .multiboot : {
        *(.multiboot)
    }

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /DISCARD/ : {
        *(.eh_frame .eh_frame.*)
        *(.note .note.*)
    }
}
