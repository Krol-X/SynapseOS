#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

#define INTERRUPTS_DESCRIPTOR_COUNT 256 
#define INTERRUPTS_KEYBOARD 33
#define INTERRUPTS_PAGING   14

typedef struct {
	uint16_t size;
	uint32_t address;
} __attribute__((packed)) IDT_t;

typedef struct {
	/* The lowest 32 bits */
	uint16_t offset_low; // offset bits 0..15
	uint16_t segment_selector; // a code segment selector in GDT or LDT
	
	/* The highest 32 bits */
	uint8_t reserved; // Just 0.
	uint8_t type_and_attr; // type and attributes
	uint16_t offset_high; // offset bits 16..31
} __attribute__((packed)) IDTDescriptor_t;

void idt_init();

// Wrappers around ASM.
void load_idt(uint32_t idt_address);
void interrupt_handler_33();
void interrupt_handler_14();

typedef struct {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebp; 
	uint32_t esi; 
	uint32_t edi; 
} __attribute__((packed)) cpu_state_t;

typedef struct {
	uint32_t error_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
} __attribute__((packed)) stack_state_t;

void interrupt_handler(cpu_state_t cpu, uint32_t interrupt, stack_state_t stack);


#endif /* _IDT_H */
