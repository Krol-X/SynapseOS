#include "include/gdt.h"
#include "include/interdesctbl.h"
#include "include/kbd.h"
#include "include/tty.h"
#include "include/qemu_log.h"
#include "include/cpu_detect.h"
#include "include/memory_manager.h"
#include "include/shell.h"

int EXIT = 0;
int DEBUG = 0;

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

	int j = 0;
	while (j != 100){
		shell_exec("cls");
		tty_printf("Loading...   %d/100", j);
		j++;
	}
	qemu_printf("memory_map %d\n", memory_map);
	shell_exec("cls");
	//tty_setcolor(VGA_COLOR_LIGHT_CYAN);
	//tty_printf("SynapseOS v0005 build %s\n\n", __TIMESTAMP__);
	shell_exec("logo");
	shell_exec("time");
	
	//init_memory_manager(memory_map);

	tty_printf("\n\n\n\n\n\n\n\n\nEnter 'help' to get info about commands\n\n");
	tty_setcolor(VGA_COLOR_LIGHT_GREEN);
	tty_printf(">");
	tty_setcolor(VGA_COLOR_LIGHT_CYAN);


	while(EXIT!=1);
}
