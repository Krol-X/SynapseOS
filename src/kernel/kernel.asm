; SynapseOS startup code
format ELF
use32
section '.text' executable
        ;multiboot spec
        align 4
        dd 0x1BADB002              ;magic
        dd 0x00                    ;flags
        dd - (0x1BADB002 + 0x00)   ;checksum. m+f+c should be zero

public start
public keyboard_handler
public load_idt
public gdt_flush

extrn kmain             ;this is defined in the c file
extrn keyboard_handler_main

load_idt:
        mov edx, [esp + 4]
        lidt [edx]
        sti                             ;turn on interrupts
        ret


keyboard_handler:   
        pushad
        cld
        call    keyboard_handler_main
        popad
        iretd

; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush(uint32_t gdt_ptr_addr);'

paging:
        ;fill page directory
        @@:
        xor di, di
        xor eax, eax
        mov eax, 0x2000 + di ;addr without flags
        mov dword[0x1000+di], eax
        inc di
        cmp di, 1023
        jne @b  
	; Загрузим значение в CR3
	mov eax, 0x1000
	mov cr3, eax
        mov eax, cr0
        or eax, 0x80000001
        mov cr0, eax
        mov eax, 0x1000
        mov cr3, eax
        mov eax, 16
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 0xFFFFDFFC
        ret
        
gdt_flush:
    cli
    mov eax, [esp + 4]
    lgdt [eax]
    mov ax, 0x10    ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush
.flush:
    ret               ; Returns back to the C code!

start:
        cli                             ;block interrupts
        mov esp, stack_space
        call kmain
        hlt                             ;halt the CPU

section '.bss' 
rb 65535 ; 64 kib for stack
stack_space: ; this is stack top, stack grows downwards