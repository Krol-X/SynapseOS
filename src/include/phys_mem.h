/*
*    SynapseOS
*    Physical memory manager
*    Allocates and deallocates physical page frames (blocks)
*/
#ifndef _PHYS_MEMORY_H_
#define _PHYS_MEMORY_H_

#include <stdint.h>
#include <stdbool.h>
#include "../include/multiboot.h"

#define PHYS_BLOCK_SIZE 4096

void pmm_parse_memory_map(multiboot_memory_map_t *mmap_addr, uint32_t length);
int pmm_find_free_block();
int pmm_find_free_blocks(uint32_t count);
uintptr_t pmm_alloc_block();
void pmm_free_block(uintptr_t addr);
bool pmm_is_block_alloced(uintptr_t addr);
uintptr_t pmm_alloc_blocks(uint32_t count);
void pmm_free_blocks(uintptr_t addr, uint32_t count);
// void pmm_alloc_chunk(uintptr_t base_addr, size_t length);
// void pmm_free_chunk(uintptr_t base_addr, size_t length);
void pmm_free_available_memory(struct multiboot_info* mb);
void update_phys_memory_bitmap_addr(uintptr_t addr);
void pmm_test();

void pmm_init(struct multiboot_info* mboot_info);

#endif /* _PHYS_MEMORY_H_ */