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


/* ------------------------------------------- */
void kmain(void *memory_map){


	qemu_printf("%c\n", (unsigned char)1);

    gdt_init(); // intialize Global Descriptor Table
	qemu_printf("Global Descriptor Table inited\n");

	idt_init(); // initialize Interrupt Descriptor Table
	qemu_printf("Interrupt Descriptor Table inited\n");
	
	kb_init(); // initialize the PS/2 keyboard
	qemu_printf("keyboard inited\n");

	qemu_printf("hello, qemu\n");

	tty_init(); // initialize terminal
	qemu_printf("tty inited\n");

	tty_printf("Loading...");
	shell_exec("cls");
	shell_exec("logo");
	shell_exec("time");
	
	//init_memory_manager(memory_map);

	tty_printf("\n\n\n\n\n\n\n\n\nEnter 'help' to get info about commands\n\n");
	tty_setcolor(VGA_COLOR_LIGHT_GREEN);
	tty_printf(">");
	tty_setcolor(VGA_COLOR_LIGHT_CYAN);


	while(EXIT!=1);
}
