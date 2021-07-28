#include "modules/gdt.h"
#include "modules/interdesctbl.h"
#include "modules/kbd.h"
#include "modules/tty.h"
#include "modules/qemu_log.h"
#include "modules/cpu_detect.h"
#include "modules/shell.h"

/* ------------------------------------------- */
void kmain(void)
{
    gdt_init(); // intialize Global Descriptor Table
	qemu_printf("Global Descriptor Table inited\n");

	idt_init(); // initialize Interrupt Descriptor Table
	qemu_printf("Interrupt Descriptor Table inited\n");
	
	kb_init(); // initialize the PS/2 keyboard
	qemu_printf("keyboard inited\n");

	qemu_printf("hello, qemu\n");

	tty_init(); // initialize terminal
	qemu_printf("tty inited\n");

	//tty_setcolor(VGA_COLOR_LIGHT_CYAN);
	//tty_printf("SynapseOS v0005 build %s\n\n", __TIMESTAMP__);
	shell_exec("logo");


	tty_printf("\n\n\nEnter 'help' to get info about commands\n\n");
	tty_setcolor(VGA_COLOR_LIGHT_GREEN);
	tty_printf("    $");
	tty_setcolor(VGA_COLOR_LIGHT_CYAN);


	while(1);
}
