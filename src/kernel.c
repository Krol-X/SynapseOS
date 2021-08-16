#include "include/gdt.h"
#include "include/interdesctbl.h"
#include "include/kbd.h"
#include "include/tty.h"
#include "include/qemu_log.h"
#include "include/cpu_detect.h"
#include "include/memory_manager.h"
#include "include/shell.h"

int EXIT = 0;

// char TEMP_MEMORY[1024];
// void *memory_map
int *memory_map = (int *)0x2C628;

/* ------------------------------------------- */
void main(){
	
	init_memory_manager(memory_map);
	VGA_MEMORY = (uint16_t*)0xB8000;
	qemu_printf(VGA_MEMORY);
	qemu_printf("Memory  Manager inited\n");
	qemu_printf("RESULT: %d\n",alloc_phys_pages(5));

    gdt_init(); // intialize Global Descriptor Table
	qemu_printf("Global Descriptor Table inited\n");

	idt_init(); // initialize Interrupt Descriptor Table
	qemu_printf("Interrupt Descriptor Table inited\n");
	
	// initialize the PS/2 keyboard
	kb_init(); 
	qemu_printf("keyboard inited\n");

	// Initialize input\output module
	tty_init();
	qemu_printf("tty inited\n");


	// Show Logo and current time using shell
	shell_exec("logo");
	shell_exec("time");
	

	// Show note and shell enter symbol
	tty_printf("\n\n\n\n\n\n\n\n\nEnter 'help' to get info about commands\n\n");
	tty_setcolor(VGA_COLOR_LIGHT_GREEN);
	tty_printf(">");
	tty_setcolor(VGA_COLOR_LIGHT_CYAN);


	// While kernel working we get input from keyboard
	while(EXIT!=1){
		check_keyboard();
	}

	// Shutdown codes	
	// qemu
	outw(0x604, 0x2000);

	// bochs
	outw(0xB004, 0x2000);

	// Virtualbox
	outw(0x4004, 0x3400);
	
	__asm__ __volatile__ ("outw %1, %0" : : "dN" ((uint16_t)0xB004), "a" ((uint16_t)0x2000));
	
}
