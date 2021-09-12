/*
*    SynapseOS
*    Physical memory manager
*    Allocates and deallocates physical page frames (blocks)
*/

#include "../include/stdlib.h"
#include "../include/phys_mem.h"
#include "../include/tty.h"
#include "../include/memmap.h"

uint32_t* phys_memory_bitmap = 0;
uint32_t phys_block_count = 0;
uint32_t phys_used_block_count = 0;

uint64_t phys_installed_memory_size = 0, phys_available_memory_size = 0;

inline static void bitmap_set(int bit) {
    phys_memory_bitmap[bit / 32] |= (1 << (bit % 32));
}

inline static void bitmap_unset(int bit) {
    phys_memory_bitmap[bit / 32] &= ~(1 << (bit % 32));
}

inline static bool bitmap_test(int bit) {
    return phys_memory_bitmap[bit / 32] & (1 << (bit % 32));
}

void pmm_parse_memory_map(multiboot_memory_map_t *mmap_addr, uint32_t length) {
	multiboot_memory_map_t *mentry = 0;	
	int i = 0;
	int n = length / sizeof(multiboot_memory_map_t); // Entries number in memory map structure

	mentry = mmap_addr; // Set pointer to memory map
	// Print info about physical memory allocation

	tty_printf("Physical memory map:\n");
	for (i = 0; i < n; i++) {
		if ((mentry + i)->type == MULTIBOOT_MEMORY_AVAILABLE) {
			tty_printf("Available: |");
			phys_available_memory_size += (mentry + i)->len_low;
		} else {
            tty_printf("Reserved:  |");
        }
		tty_printf(" addr: %x", (mentry + i)->addr_low);
		tty_printf(" length: %x\n", (mentry + i)->len_low);
		phys_installed_memory_size += (mentry + i)->len_low;
	}
	tty_printf("Installed memory size: %d KB", phys_installed_memory_size/1024);
	tty_printf(" = %d MB\n", phys_installed_memory_size/(1024*1024));
	tty_printf("Available memory size: %d KB", phys_available_memory_size/1024);
	tty_printf(" = %d MB\n", phys_available_memory_size/(1024*1024));
}

// Finds first free block in bitmap
// returns index of block, or -1 if not found
int pmm_find_free_block() {
    for (uint32_t i = 0; i < phys_block_count / 32; i++) {
        uint32_t block = phys_memory_bitmap[i];
        if (block != 0xFFFFFFFF) {
            for (uint8_t j = 0; j < 32; j++) {
                int bit = 1 << j;
                if (!(bit & block)) {
                    return (32 * i) + j;
                }
            }
        }
    }
    return -1;
}


// Finds first contignous range of free <count> blocks in bitmap
// returns index of first block of range, or -1 if not found
int pmm_find_free_blocks(uint32_t count) {
    int starting_block = -1;
    int starting_block_bit = -1;
    uint32_t cur_block_num = 0;
    for (uint32_t i = 0; i < phys_block_count / 32; i++) {
        uint32_t cur_block = phys_memory_bitmap[i];
        if (cur_block == 0xFFFFFFFF) {
            cur_block_num = 0;
            continue;
        }
        for (uint8_t j = 0; j < 32; j++) { // scan all of 32 bits of cur_block
            int bit = 1 << j;
            if (bit & cur_block) {   // bit is set
                cur_block_num = 0;
                continue;
            }
            if (!cur_block_num) starting_block = i;
            if (!cur_block_num) starting_block_bit = j;
            cur_block_num += 1;
            if (cur_block_num == count) {
                return (32 * starting_block) + starting_block_bit;
            }
        }
    }
    return -1;
}

// Allocate one block in physical memory
void* pmm_alloc_block() {
    if (phys_block_count - phys_used_block_count <= 0) {
        return (void*)0xFFFFFFFF;
    }
    int free_block = pmm_find_free_block();
    if (free_block == -1) {
        return (void*)0xFFFFFFFF;
    }
    bitmap_set(free_block);
    phys_used_block_count++;
    return (void*)(free_block * PHYS_BLOCK_SIZE);
}

// FRee block in physical memory
void pmm_free_block(void *addr) {
    int block = (int)addr / PHYS_BLOCK_SIZE;
    bitmap_unset(block);
    phys_used_block_count--;
}

bool pmm_is_block_alloced(void *addr) {
    int block = (int)addr / PHYS_BLOCK_SIZE;
    return bitmap_test(block);
}

// Allocate contignous range of <count> blocks in physical memory
void* pmm_alloc_blocks(uint32_t count) {
    if (phys_block_count - phys_used_block_count <= 0) {
        return (void*)0xFFFFFFFF;
    }
    int free_block = pmm_find_free_blocks(count);
    if (free_block == -1) {
        return (void*)0xFFFFFFFF;
    }
    for (uint32_t i = 0; i < count; i++) {
        bitmap_set(free_block + i);
    }
    phys_used_block_count += count;
    return (void*)(free_block * PHYS_BLOCK_SIZE);
}

// Free contignous range of physical blocks
void pmm_free_blocks(void *addr, uint32_t count) {
    int block = (int)addr / PHYS_BLOCK_SIZE;
    for (uint32_t i = 0; i < count; i++) {
        bitmap_unset(block + i);
    }
    phys_used_block_count -= count;
}

// Internal functions to allocate ranges of memory:

void pmm_alloc_chunk(void *base_addr, size_t length) {
    // tty_printf("pmm_alloc_chunk\n");
    int cur_block_addr = (int)base_addr / PHYS_BLOCK_SIZE;
    int num_blocks = length / PHYS_BLOCK_SIZE;
    while (num_blocks-- >= 0)
    {
        bitmap_set(cur_block_addr++);
        phys_used_block_count--;
    }
}

void pmm_free_chunk(void *base_addr, size_t length) {
    int cur_block_addr = (int)base_addr / PHYS_BLOCK_SIZE;
    int num_blocks = length / PHYS_BLOCK_SIZE;

    while (num_blocks--)
    {
        bitmap_unset(cur_block_addr++);
        phys_used_block_count--;
    }
}

// Functions to initialize the pmm:

// Free all memory marked as available by memory map
void pmm_free_available_memory(struct multiboot_info* mb) {
    multiboot_memory_map_t* mm = (multiboot_memory_map_t*)mb->mmap_addr;
    while ((unsigned int)mm < mb->mmap_addr + mb->mmap_length) {
        //tty_printf("freed\n");
        if (mm->type == MULTIBOOT_MEMORY_AVAILABLE) {
            pmm_free_chunk((void*)mm->addr_low, mm->len_low);
        }
        mm = (multiboot_memory_map_t*)((unsigned int)mm + mm->size + sizeof(mm->size));
    }
    bitmap_set(0);
}

// PMM initialization
void pmm_init(struct multiboot_info* mboot_info) {
	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mboot_info->mmap_addr;
	pmm_parse_memory_map(mmap, mboot_info->mmap_length); // it also calculates the phys_installed_memory_size

    phys_block_count = phys_installed_memory_size / PHYS_BLOCK_SIZE; // how many blocks will be
    phys_used_block_count = phys_block_count; // initially all blocks are used
    phys_memory_bitmap = (uint32_t*)KERNEL_END_PADDR; // physical memory bitmap starts after kernel
    memset(phys_memory_bitmap, 0xFF, phys_block_count / 8); // initially we mark all installed memory as used
    
    // tty_printf("Total blocks: %d\n", phys_block_count);

    // Frees memory GRUB considers available
    pmm_free_available_memory(mboot_info);

    // From the freed memory, we need to allocate the ones used by the Kernel
    pmm_alloc_chunk(KERNEL_START_PADDR, KERNEL_SIZE);

    tty_printf("KERNEL_START_PADDR = %x, KERNEL_END_PADDR = %x, KERNEL_SIZE = %d bytes ", KERNEL_START_PADDR, KERNEL_END_PADDR, KERNEL_SIZE);
    // tty_printf("MemMap addr = %x\n", mboot_info->mmap_addr);
    
    // We also need to allocate the memory used by the Physical Map itself
    pmm_alloc_chunk(phys_memory_bitmap, phys_block_count);
    void *kernel_phys_map_start = phys_memory_bitmap;
    void *kernel_phys_map_end = kernel_phys_map_start + (phys_block_count / 8);

    tty_printf("Physical memory manager installed. Physical memory bitmap start: %x, end: %x, size = %d bytes\n", kernel_phys_map_start, kernel_phys_map_end, kernel_phys_map_end - kernel_phys_map_start);
}

void pmm_test() {
    tty_printf("------\nPMM TEST:\n");
    void *myptr = pmm_alloc_block();
    tty_printf("  myptr = %x\n", myptr);
    char *str = "Hello world!";
    memcpy(myptr, str, 13);
    char buf[25];
    memcpy(buf, myptr, 13);
    tty_printf("  read from myptr string = %s\n------\n", buf);
}

void update_phys_memory_bitmap_addr(void *addr) {
    phys_memory_bitmap = (uint32_t*)addr;
}