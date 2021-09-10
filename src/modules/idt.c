//Interrupt Descriptor_ able

#include "../include/idt.h"
#include "../include/gdt.h"
#include "../include/kbd.h"
#include "../include/ports.h"

//Interrupt Descriptor Table || Таблица векторов прерываний


#define KEYBOARD_INTERRUPT 0x21

struct IDT_entry IDT[IDT_SIZE]; // array of IDT entries

void idt_init(void) // initialize IDT
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[KEYBOARD_INTERRUPT].offset_lowerbits = keyboard_address & 0xffff;
	IDT[KEYBOARD_INTERRUPT].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[KEYBOARD_INTERRUPT].zero = 0;
	IDT[KEYBOARD_INTERRUPT].type_attr = INTERRUPT_GATE;
	IDT[KEYBOARD_INTERRUPT].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	outb(0x20 , 0x11);
	outb(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	outb(KEYBOARD_INTERRUPT , 0x20);
	outb(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	outb(KEYBOARD_INTERRUPT , 0x00);
	outb(0xA1 , 0x00);

	/* ICW4 - environment info */
	outb(KEYBOARD_INTERRUPT , 0x01);
	outb(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	outb(KEYBOARD_INTERRUPT , 0xff);
	outb(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr); // load IDT to special cpu register
}
