#ifndef _MEMMAP_H_
#define _MEMMAP_H_

#include <stdint.h>

// Constants to the boundaries of memory
#define KERNEL_START_PADDR ((uintptr_t)&kernel_phys_start)
#define KERNEL_END_PADDR ((uintptr_t)&kernel_phys_end)
#define KERNEL_SIZE (KERNEL_END_PADDR - KERNEL_START_PADDR)

#define KERNELSPACE_VADDR 0xC0000000

#define KERNEL_START_VADDR ((uint32_t)&kernel_virt_start)
#define KERNEL_END_VADDR ((uint32_t)&kernel_virt_end)

#define KERNEL_PHYS_MAP_START kernel_phys_map_start
#define KERNEL_PHYS_MAP_END kernel_phys_map_end
#define KERNEL_PHYS_MAP_SIZE kernel_phys_map_end - kernel_phys_map_start;

// Constants for the Physical Memory Manager
#define PHYS_BLOCKS_PER_BYTE 8
#define PHYS_BLOCK_SIZE 4096

// Constants for the Virtual Memory Manager
#define TEMP_PAGE_ADDR 0xC03FF000 //we reserve 0x1000 i.e 4096 bytes up to 0xC0400000 for temporary page, because kernel in memory takes < 4 MB now, and for addresses 0xC0000000 - 0xC0400000 we have pte allocated
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR 1024
#define PAGE_SIZE 4096
#define PAGE_ENTRIES 1024

// Constants for Kernel heap manager
// #define KHEAP_START_VADDR 0xC0500000 // if kernel size > 4MB, change

// Constants for User heap manager
// #define UHEAP_START_VADDR 0x20000000 // virtual address where user heap in each process page dir begins

// Functions to
#define ALIGN_BLOCK(addr) (addr) - ((addr) % PHYS_BLOCK_SIZE);

// Addresses of the Kernel in the Physical and Virtual memory
extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;
extern uint32_t kernel_virt_start;
extern uint32_t kernel_virt_end;

// Addresses of the Physical Memory Map in the Physical memory
extern uintptr_t kernel_phys_map_start;
extern uintptr_t kernel_phys_map_end;


#endif