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
public read_port
public write_port
public load_idt
public gdt_flush

extrn kmain 		;this is defined in the c file
extrn keyboard_handler_main

read_port:
	mov edx, [esp + 4]
			;al is the lower 8 bits of eax
	in al, dx	;dx is the lower 16 bits of edx
	ret

write_port:
	mov   edx, [esp + 4]    
	mov   al, [esp + 4 + 4]  
	out   dx, al  
	ret

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti 				;turn on interrupts
	ret

; TODO need pusha / popa, preserving DF
keyboard_handler:                 
	call    keyboard_handler_main
	iretd

; This will set up our new segment registers. We need to do
; something special in order to set CS. We do what is called a
; far jump. A jump that includes a segment as well as an offset.
; This is declared in C as 'extern void gdt_flush(uint32_t gdt_ptr_addr);'
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
	cli 				;block interrupts
	mov esp, stack_space
	call kmain
	hlt 				;halt the CPU

section '.bss' 
stack_space rb 65535 ; 64 kib for stack