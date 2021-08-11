#ifndef IDT_H
#define IDT_H

#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

void load_idt(unsigned long *idt_ptr);
void idt_init(void);

extern struct IDT_entry IDT[IDT_SIZE];

#endif // IDT_H