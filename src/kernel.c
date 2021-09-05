#include "include/gdt.h"
#include "include/interdesctbl.h"
#include "include/kbd.h"
#include "include/tty.h"
#include "include/qemu_log.h"
#include "include/cpu_detect.h"
#include "include/memory_manager.h"
#include "include/shell.h"
#include "include/multiboot.h"
#include "include/time.h"

int EXIT = 0;

/* ------------------------------------------- */
void main(multiboot_info_t* mbd, unsigned int magic){
	uint16_t* VGA_MEMORY = (uint16_t*)0xC03FF000;

	qemu_printf("magic x: %x\n",magic);
	qemu_printf("magic d: %d\n",magic);
	qemu_printf("MULTIBOOT_BOOTLOADER_MAGIC x: %x\n",MULTIBOOT_BOOTLOADER_MAGIC);
	qemu_printf("MULTIBOOT_BOOTLOADER_MAGIC d: %d\n",MULTIBOOT_BOOTLOADER_MAGIC);
	qemu_printf("VGA MEMORY: %d\n",VGA_MEMORY);
	qemu_printf("Memory  Manager inited\n");
	qemu_printf("RESULT: %d\n",alloc_phys_pages(1));

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

	init_memory_manager(mbd);
	/* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid magic number!");
    }
	
	/* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        panic("invalid memory map given by GRUB bootloader");
    }
	/*
	 Loop through the memory map and display the values 
    unsigned int i;
    for(i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
 
        qemu_printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
            mmmt->addr_low, mmmt->len_low, mmmt->size, mmmt->type);
		tty_printf("\nStart Addr: %x | Length: %x | Size: %x | Type: %d\n",
            mmmt->addr_low, mmmt->len_low, mmmt->size, mmmt->type);
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            
        }
    }*/
	
	qemu_printf("flags = %d\n", mbd->flags);
	qemu_printf("mem_lower = %d\n", mbd->mem_lower);
	qemu_printf("mem_upper = %d\n", mbd->mem_upper);
	qemu_printf("boot_device = %d\n", mbd->boot_device);
	qemu_printf("cmdline = %d\n", mbd->cmdline);
	qemu_printf("mmap_length = %d\n", mbd->mmap_length);
	qemu_printf("mmap_addr = %d\n", mbd->mmap_addr);
	qemu_printf("drives_length = %d\n", mbd->drives_length);
	qemu_printf("drives_addr = %d\n", mbd->drives_addr);
	qemu_printf("config_table = %d\n", mbd->config_table);
	qemu_printf("config_table = %d\n", mbd->config_table);
	qemu_printf("apm_table = %d\n", mbd->apm_table);
	qemu_printf("vbe_control_info = %d\n", mbd->vbe_control_info);
	// Show note and shell enter symbol
	tty_printf("\n\nEnter 'help' to get info about commands\n\n");
	tty_setcolor(VGA_COLOR_LIGHT_GREEN);
	tty_printf(">");
	tty_setcolor(VGA_COLOR_LIGHT_CYAN);

	long long int lifetime;
	// While kernel working we get input from keyboard
	while(EXIT!=1){
		check_keyboard();
		lifetime++;
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
