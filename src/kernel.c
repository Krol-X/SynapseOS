#include "include/gdt.h"
#include "include/interdesctbl.h"
#include "include/kbd.h"
#include "include/tty.h"
#include "include/qemu_log.h"
#include "include/cpu_detect.h"
#include "include/memory_manager.h"
#include "include/shell.h"

int EXIT = 0;

//char TEMP_MEMORY[1024];
//void *memory_map
int *memory_map = (int *)0x2C628;

/* ------------------------------------------- */
void kmain(){
	
	init_memory_manager(memory_map);
	VGA_MEMORY = (uint16_t*)0xB8000;
	qemu_printf(VGA_MEMORY);
	qemu_printf("Memory  Manager inited\n");
	qemu_printf("RESULT: %d\n",alloc_phys_pages(5));

    gdt_init(); // intialize Global Descriptor Table
	qemu_printf("Global Descriptor Table inited\n");

	idt_init(); // initialize Interrupt Descriptor Table
	qemu_printf("Interrupt Descriptor Table inited\n");
	
	kb_init(); // initialize the PS/2 keyboard
	qemu_printf("keyboard inited\n");

	tty_init(); // initialize terminal
	qemu_printf("tty inited\n");


	shell_exec("logo");
	
	shell_exec("time");
	
	//init_memory_manager(memory_map);

	tty_printf("\n\n\n\n\n\n\n\n\nEnter 'help' to get info about commands\n\n");
	tty_setcolor(VGA_COLOR_LIGHT_GREEN);
	tty_printf(">");
	tty_setcolor(VGA_COLOR_LIGHT_CYAN);


	while(EXIT!=1){
		check_keyboard();
	}

	/*
	//qemu
	outw(0x604, 0x2000);

	//bochs
	outw(0xB004, 0x2000);

	//Virtualbox
	outw(0x4004, 0x3400);

	asm("mov ax, 0x5307");
	asm("mov bx, 0x0001");
	asm("mov cx, 0x0003");
	asm("int 0x15");
	*/
	__asm__ __volatile__ ("outw %1, %0" : : "dN" ((uint16_t)0xB004), "a" ((uint16_t)0x2000));
	
}
