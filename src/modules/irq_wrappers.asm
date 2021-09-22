format ELF
use32

extrn interrupt_handler

macro no_error_code_interrupt_handler num
{
interrupt_handler_#num:
	push	dword 0                      ; push 0 as error code
	push	dword num                    ; push the interrupt number
	jmp	    common_interrupt_handler     ; jump to the common handler
}

macro error_code_interrupt_handler num
{
interrupt_handler_#num:
	push    dword  num                    ; push the interrupt number
	jmp     common_interrupt_handler      ; jump to the common handler
}

section '.text' executable

common_interrupt_handler:                ; the common parts of the generic interrupt handler
	; save the registers
	push    eax
	push    ebx
	push	ecx
	push	edx
	push	ebp
	push	esi
	push	edi

    ; call the C function
    call    interrupt_handler

    ; restore the registers
	pop	edi
	pop	esi
	pop	ebp
	pop	edx
	pop	ecx
	pop	ebx
    pop     eax

	; restore the esp
	add     esp, 8

	; return to the code that got interrupted
	iret

no_error_code_interrupt_handler	33	; create handler for interrupt 1 (keyboard)
no_error_code_interrupt_handler 14	; create handler for interrupt 2 (paging)

public interrupt_handler_14
public interrupt_handler_33
   