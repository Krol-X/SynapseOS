#include "include/gdt.h"
#include "include/interdesctbl.h"
#include "include/kbd.h"
#include "include/tty.h"
#include "include/qemu_log.h"
#include "include/cpu_detect.h"
#include "include/memory_manager.h"
#include "include/shell.h"
#include "include/multiboot.h"

int EXIT = 0;

// char TEMP_MEMORY[1024];
// void *memory_map
int *memory_map = (int *)0x2C628;

/* ------------------------------------------- */
void main(multiboot_info_t* mbd, unsigned int magic){
	
	
	init_memory_manager(memory_map);
	VGA_MEMORY = (uint16_t*)0xB8000;
	qemu_printf("magic: %d\n",magic);
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

	// Show Logo and current time using shell
	shell_exec("logo");
	shell_exec("time");
	
	/* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        panic("invalid magic number!");
    }
	
	/* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        panic("invalid memory map given by GRUB bootloader");
    }
	/* Loop through the memory map and display the values */
    int i;
    for(i = 0; i < mbd->mmap_length; 
        i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
 
        tty_printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
            mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            /* 
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually 
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
        }
    }
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
