#include "include/gdt.h"
#include "include/idt.h"
#include "include/kbd.h"
#include "include/tty.h"
#include "include/vga.h"
#include "include/qemu_log.h"
#include "include/cpu_detect.h"
#include "include/phys_mem.h"
#include "include/virt_mem.h"
#include "include/shell.h"
#include "include/multiboot.h"
#include "include/time.h"
#include "include/stdlib.h"

int EXIT = 0;

void main(multiboot_info_t* mboot_info, unsigned int magic) {
	int eip;
	asm volatile("1: lea 1b, %0;": "=a"(eip));
    qemu_printf("eip in the beginning of main() = %x\n", eip);

	/* Make sure the magic number matches for memory mapping*/
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid magic number!");
    }
	/* Check bit 6 to see if we have a valid memory map */
    if (!(mboot_info->flags >> 6 & 0x1)) {
        panic("invalid memory map given by GRUB bootloader");
    }

	// qemu_printf("magic x: %x\n", magic);
	// qemu_printf("magic d: %d\n", magic);
	// qemu_printf("MULTIBOOT_BOOTLOADER_MAGIC x: %x\n",MULTIBOOT_BOOTLOADER_MAGIC);
	// qemu_printf("MULTIBOOT_BOOTLOADER_MAGIC d: %d\n",MULTIBOOT_BOOTLOADER_MAGIC);

    gdt_init(); // intialize Global Descriptor Table
	qemu_printf("Global Descriptor Table inited\n");

	idt_init(); // initialize Interrupt Descriptor Table
	qemu_printf("Interrupt Descriptor Table inited\n");
	
	// initialize the PS/2 keyboard
	kb_init(); 
	qemu_printf("keyboard initialized\n");

	// Initialize input\output module
	tty_init();
	qemu_printf("tty initialized\n");

	// Show Logo and current time using shell
	shell_exec("logo");
	shell_exec("time");

	pmm_init(mboot_info);
	qemu_printf("Physical memory manager initialized\n");

	vmm_init();
	qemu_printf("Virtual memory manager initialized\n");

	vmm_test();
	
	tty_printf("\nEnter 'help' to get info about commands\n\n");
	tty_setcolor(VGA_COLOR_LIGHT_GREEN);
	tty_printf(">");
	tty_setcolor(VGA_COLOR_LIGHT_CYAN);

	//long long int lifetime = 0;
	// While kernel working we get input from keyboard
	while(EXIT!=1){
		check_keyboard();
		io_wait();
		//lifetime++;
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
