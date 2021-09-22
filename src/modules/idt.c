//Interrupt Descriptor table module

#include "../include/idt.h"
#include "../include/gdt.h"
#include "../include/ports.h"
#include "../include/pic.h"

#include "../include/kbd.h"
#include "../include/virt_mem.h"

IDTDescriptor_t idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
IDT_t idt;

void idt_init_descriptor(int index, uint32_t address) {
	idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF; // offset bits 0..15
	idt_descriptors[index].offset_low = (address & 0xFFFF); // offset bits 16..31

	idt_descriptors[index].segment_selector = 0x08; // The second (code) segment selector in GDT: one segment is 64b.
	idt_descriptors[index].reserved = 0x00; // Reserved.

	/*
	   Bit:     | 31              16 | 15 | 14 13 | 12 | 11     10 9 8   | 7 6 5 | 4 3 2 1 0 |
	   Content: | offset high        | P  | DPL   | S  | D and  GateType | 0 0 0 | reserved
		P	If the handler is present in memory or not (1 = present, 0 = not present). Set to 0 for unused interrupts or for Paging.
		DPL	Descriptor Privilige Level, the privilege level the handler can be called from (0, 1, 2, 3).
		S	Storage Segment. Set to 0 for interrupt gates.
		D	Size of gate, (1 = 32 bits, 0 = 16 bits).
	*/
	idt_descriptors[index].type_and_attr =	(0x01 << 7) |			// P
						(0x00 << 6) | (0x00 << 5) |	// DPL
						0xe;				// 0b1110=0xE 32-bit interrupt gate
}

void idt_init() {
	idt_init_descriptor(INTERRUPTS_KEYBOARD, (unsigned int) interrupt_handler_33);
	idt_init_descriptor(INTERRUPTS_PAGING, (unsigned int) interrupt_handler_14);

	idt.address = (int) &idt_descriptors;
	idt.size = sizeof(IDTDescriptor_t) * INTERRUPTS_DESCRIPTOR_COUNT;
	load_idt((uint32_t)&idt);

	/*pic_remap(PIC_PIC1_OFFSET, PIC_PIC2_OFFSET);*/
	pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
}

void interrupt_handler(__attribute__((unused)) cpu_state_t cpu,
	uint32_t interrupt,
	__attribute__((unused)) stack_state_t stack
) {
	// uint8_t scan_code;
	// uint8_t ascii;

	switch (interrupt) {
		case INTERRUPTS_KEYBOARD:
			// scan_code = keyboard_read_scan_code();
			// if (scan_code <= KEYBOARD_MAX_ASCII) {
			// 	ascii = keyboard_scan_code_to_ascii(scan_code);
			// 	serial_configure_baud_rate(SERIAL_COM1_BASE, 4);
			// 	serial_configure_line(SERIAL_COM1_BASE);
			// 	char str[1];
			// 	str[0] = ascii;
			// 	serial_write(str, 1);
			// }
			// pic_acknowledge(interrupt);
			keyboard_handler_main();
			break;
		
		case INTERRUPTS_PAGING:
			page_fault_handler();	
			break;
		default:
			break;
    }
}