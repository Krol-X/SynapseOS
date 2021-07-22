format ELF
public _start
extrn kernel_main

section ".text" executable

_start:
    movzx edx, dl
    push edx
    push esi
    push ebx
    lgdt [gdtr]
    call kernel_main
@@:
    ;cli
    ;hlt
    jmp @b

section ".data" writable

gdt:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
    gdtr:
    dw $ - gdt
    dd gdt