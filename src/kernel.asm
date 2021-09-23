; SynapseOS startup code
format ELF
use32

section '.multiboot.data' align 4096
	; multiboot header
	align 4
	dd 0x1BADB002	  ;magic
	dd 0x00      ;flags
	dd - (0x1BADB002 + 0x00)   ;checksum. m+f+c should be zero

	align 4096
	_boot_page_dir:
	rb 1024*4

	rb 4096
	early_stack: ; stack top, which we use before enabling boot page directory

KERNELSPACE_BASE = 0xC0000000

public start
public keyboard_handler
public load_idt
public gdt_flush
public enable_paging

;public before_bpd
;public after_bpd

extrn main	 ;this is defined in the c file
extrn keyboard_handler_main

extrn qemu_puthex
extrn com1_write_char

section '.bootstrap_stack' 
rb 65535 ; 64 kib for stack
stack_space: ; this is stack top, stack grows downwards

section '.multiboot.text' executable ;align 4096

load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti      ;turn on interrupts
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
gdt_flush:
	cli
	mov eax, [esp + 4]
	lgdt [eax]
	mov ax, 0x10	; 0x10 is the offset in the GDT to our data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:.flush
.flush:
	ret	  ; Returns back to the C code!

start:
	cli
	mov esp, early_stack
	mov esi, eax
	mov edi, ebx

	mov ebx, 0x00000083
	xor ecx, ecx

; before_bpd:

; mov eax, _boot_page_dir ;;;
; mov ebx, early_stack ;; 
; hlt ;; 

.fill_bpd:
	cmp ecx, 1024
	jae .fill_bpd_end

	; the rest virt 4MB pages are mapped to identical physical 4MB frames
	mov dword [_boot_page_dir + ecx*4], ebx

	; if we reached to virt page, responsible for kernel space virtual base
	; then map it to physical first 4MB frame
	cmp ecx, KERNELSPACE_BASE shr 22 
	jne @f
	mov dword [_boot_page_dir + ecx*4], 0x00000083
@@:
	add ebx, 0x00400000
	inc ecx
	jmp .fill_bpd
.fill_bpd_end:

;after_bpd:

	; bpd + 4092 i.e we write address of page_dir|0000003 to last pde
	; it is recursize mapping techniquem used to accessign to pde's and pte's
	mov dword [_boot_page_dir + 0xFFC], _boot_page_dir
	or  dword [_boot_page_dir + 0xFFC], 0x3

	; Load Page Directory Base Register.
	mov ecx, _boot_page_dir
	mov cr3, ecx

	; Set PSE bit in CR4 to enable 4MB pages.
	mov ecx, cr4
	or  ecx, 0x00000010
	mov cr4, ecx

	; Set PG bit in CR0 to enable paging.
	mov ecx, cr0
	or  ecx, 0x80000000
	mov cr0, ecx

	mov esp, stack_space
	push esi ; Multiboot magic number
	push edi ; Multiboot structure

	mov eax, main
	;hlt ;;;;
	call eax

	hlt

section '.text' executable
enable_paging:
	; Move directory into CR3
	mov eax, [esp + 4]
	mov cr3, eax

	; Disable 4MB pages
	mov ecx, cr4
	and ecx, not 0x00000010
	mov cr4, ecx
	
	; Enable paging
	mov eax, cr0
	or  eax, 0x80000000
	mov cr0, eax

	;;hlt ;; 
	ret

