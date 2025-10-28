/*
 * Algae linker script for AMD64 port
 * Copyright (c) 2025, Ian Moffett and the Algae team.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS
{
    text          PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ;
    rodata        PT_LOAD    FLAGS((1 << 2)) ;
    data          PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ;
}

SECTIONS
{
    . = 0xFFFFFFFF80000000;

    .text : {
        *(.text .text.*)
    } :text

    . += CONSTANT(MAXPAGESIZE);

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    .modules : {
        __modules_init_start = .;
        KEEP(*(.modules .modules))
        __modules_init_end = .;
    } :rodata

    .trampoline : {
        *(.trampoline .trampoline.*)
    } :rodata

    . += CONSTANT(MAXPAGESIZE);

    .data : {
        *(.data)
    } :data

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    . = ALIGN(64);
    .data.cacheline_aligned : {
        *(.data.cacheline_aligned)
    }

    /DISCARD/ : {
        *(.eh_frame .eh_frame.*)
        *(.note .note.*)
    }
}
