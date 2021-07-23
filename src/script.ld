OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS {
    .text 0xFFC00000 : {
        *(.text)
        *(.code)
        *(.rodata*)
    }
    .data ALIGN(0x1000) : {
        *(.data)
    }
        .bss ALIGN(0x1000) : {
        *(.bss)
    }
    .empty ALIGN(0x1000) - 1 : {
        BYTE(0)
    }
}