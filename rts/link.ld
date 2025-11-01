ENTRY(_start)

SECTIONS {
    . = 0x1000;

    .idata : {
        *(.idata)
    }

    .rdata : {
        *(.rdata)
    }

    .bss : {
        *(.bss .bss.*)
    }
}
