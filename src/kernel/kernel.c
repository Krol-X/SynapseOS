#include "Modules/gdt.h"
#include "Modules/Interrupt_Descriptor_Table.h"
#include "Modules/kbd.h"
#include "Modules/tty.h"
#include "Modules/qemu_log.h"
#include "Modules/cpu_detect.h"

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

	tty_setcolor(VGA_COLOR_LIGHT_CYAN);
	tty_printf("SynapseOS v0003 build %s\n\n", __TIMESTAMP__);
	tty_setcolor(VGA_COLOR_LIGHT_GREY);

	detect_cpu();

	tty_putstring_color("\nType something on keyboard\n\n", VGA_COLOR_GREEN);
	tty_setcolor(VGA_COLOR_LIGHT_MAGENTA);

	while(1);
}
