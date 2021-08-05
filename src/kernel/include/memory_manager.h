#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "string.h"
#include <stdint-gcc.h>

#define PAGE_SIZE 0x1000
#define PAGE_OFFSET_BITS 12
#define PAGE_OFFSET_MASK 0xFFF
#define PAGE_TABLE_INDEX_BITS 10
#define PAGE_TABLE_INDEX_MASK 0x3FF
#define PHYADDR_BITS 32
#define PAGE_VALID 1
#define PAGE_WRITABLE 2
#define PAGE_USER 4


typedef size_t phyaddr;


#define KERNEL_BASE 0xFFC00000
#define KERNEL_PAGE_TABLE 0xFFFFE000
#define TEMP_PAGE 0xFFFFF000
#define TEMP_PAGE_INFO (KERNEL_PAGE_TABLE + ((TEMP_PAGE >> PAGE_OFFSET_BITS) & PAGE_TABLE_INDEX_MASK) * sizeof(phyaddr))


phyaddr kernel_page_dir;
size_t memory_size;

// must be aligned to (at least)0x20
uint64_t page_dir_ptr_tab[4] __attribute__((aligned(0x20))); 
// 512 entries
uint64_t page_dir[512] __attribute__((aligned(0x1000)));  // must be aligned to page boundary


void init_memory_manager();
#endif
