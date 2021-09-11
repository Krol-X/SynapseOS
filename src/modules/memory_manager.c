//  Memory manager module

//Manage memory

#include "../include/memory_manager.h"
#include "../include/qemu_log.h"


/*typedef struct {
    uint64 base;
    uint64 length;
    uint32 type;
    uint32 acpi_ext_attrs;
} __attribute__((packed)) MemoryMapEntry;*/


typedef struct {
    phyaddr next;
    phyaddr prev;
    size_t size;
} PhysMemoryBlock;


size_t free_page_count = 0;
phyaddr free_phys_memory_pointer = -1;

void enable_paging(){
    return;
}

void init_memory_manager(multiboot_info_t *mb_info) {
    asm("movl %%cr3, %0":"=a"(kernel_page_dir));
    //memory_size = 0x100000;
    memory_size = 0;
    unsigned int i;
    for(i = 0; i < mb_info->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*) (mb_info->mmap_addr + i);
        if ((entry->type == 1) && (entry->addr_low >= 0x100000)) {
            free_phys_pages(entry->addr_low, entry->len_low >> PAGE_OFFSET_BITS);
            memory_size += entry->len_low;
        }
    }
}


/* Physical memory manager */
void temp_map_page(phyaddr addr) {
    *((phyaddr*)TEMP_PAGE_INFO) = (addr & ~PAGE_OFFSET_MASK) | PAGE_VALID | PAGE_WRITABLE;
    asm("invlpg (,%0,)"::"a"(TEMP_PAGE));
}


size_t get_free_memory_size() {
    return free_page_count << PAGE_OFFSET_BITS;
}


phyaddr alloc_phys_pages(size_t count) {
    if (free_page_count < count) return -1;
        phyaddr result = -1;
    if (free_phys_memory_pointer != (unsigned int)-1) {
        phyaddr cur_block = free_phys_memory_pointer;
        do {
            temp_map_page(cur_block);
            if (((PhysMemoryBlock*)TEMP_PAGE)->size == count) {
                phyaddr next = ((PhysMemoryBlock*)TEMP_PAGE)->next;
                phyaddr prev = ((PhysMemoryBlock*)TEMP_PAGE)->prev;

                temp_map_page(next);
                ((PhysMemoryBlock*)TEMP_PAGE)->prev = prev;

                temp_map_page(prev);
                ((PhysMemoryBlock*)TEMP_PAGE)->next = next;

                if (cur_block == free_phys_memory_pointer) {
                    free_phys_memory_pointer = next;
                    if (cur_block == free_phys_memory_pointer) {
                        free_phys_memory_pointer = -1;
                    }
                }
                result = cur_block;
                break;
            } else if (((PhysMemoryBlock*)TEMP_PAGE)->size > count) {
                ((PhysMemoryBlock*)TEMP_PAGE)->size -= count;
                result = cur_block + (((PhysMemoryBlock*)TEMP_PAGE)->size << PAGE_OFFSET_BITS);
                break;
            }
        cur_block = ((PhysMemoryBlock*)TEMP_PAGE)->next;
        } while (cur_block != free_phys_memory_pointer);

        if (result != (unsigned int)-1) {
            free_page_count -= count;
        }
    }
    return result;
}


void free_phys_pages(phyaddr base, size_t count) {
    qemu_printf("\nbase: %x, count: %x", base, count);
    
    if (free_phys_memory_pointer == (unsigned int)-1) {
        temp_map_page(base);
        ((PhysMemoryBlock*)TEMP_PAGE)->next = base;
        ((PhysMemoryBlock*)TEMP_PAGE)->prev = base;
        ((PhysMemoryBlock*)TEMP_PAGE)->size = count;
        free_phys_memory_pointer = base;
    } else {
        phyaddr cur_block = free_phys_memory_pointer;
        qemu_printf("\nTry free memory: ");

        do {
            temp_map_page(cur_block);
            if (cur_block + (((PhysMemoryBlock*)TEMP_PAGE)->size << PAGE_OFFSET_BITS) == base) {
                ((PhysMemoryBlock*)TEMP_PAGE)->size += count;
                if (((PhysMemoryBlock*)TEMP_PAGE)->next == base + (count << PAGE_OFFSET_BITS)) {
                    phyaddr next1 = ((PhysMemoryBlock*)TEMP_PAGE)->next;
                    temp_map_page(next1);
                    phyaddr next2 = ((PhysMemoryBlock*)TEMP_PAGE)->next;
                    size_t new_count = ((PhysMemoryBlock*)TEMP_PAGE)->size;
                    temp_map_page(next2);((PhysMemoryBlock*)TEMP_PAGE)->prev = cur_block;
                    temp_map_page(cur_block);
                    ((PhysMemoryBlock*)TEMP_PAGE)->next = next2;
                    ((PhysMemoryBlock*)TEMP_PAGE)->size += new_count;
                }
                qemu_printf("\n1 end");
                break;
            } else if (base + (count << PAGE_OFFSET_BITS) == cur_block){
                size_t old_count = ((PhysMemoryBlock*)TEMP_PAGE)->size;
                phyaddr next = ((PhysMemoryBlock*)TEMP_PAGE)->next;
                phyaddr prev = ((PhysMemoryBlock*)TEMP_PAGE)->prev;
                temp_map_page(next);
                ((PhysMemoryBlock*)TEMP_PAGE)->prev = base;
                temp_map_page(prev);
                ((PhysMemoryBlock*)TEMP_PAGE)->next = base;
                temp_map_page(base);
                ((PhysMemoryBlock*)TEMP_PAGE)->next = next;
                ((PhysMemoryBlock*)TEMP_PAGE)->prev = prev;
                ((PhysMemoryBlock*)TEMP_PAGE)->size = count + old_count;
                qemu_printf("\nbase + (count << PAGE_OFFSET_BITS) == cur_block");
                break;
            } else if ((cur_block > base) || (((PhysMemoryBlock*)TEMP_PAGE)->next == free_phys_memory_pointer)) {
                phyaddr prev = ((PhysMemoryBlock*)TEMP_PAGE)->next;
                ((PhysMemoryBlock*)TEMP_PAGE)->prev = base;
                temp_map_page(prev);
                ((PhysMemoryBlock*)TEMP_PAGE)->next = base;
                temp_map_page(base);
                ((PhysMemoryBlock*)TEMP_PAGE)->next = cur_block;
                ((PhysMemoryBlock*)TEMP_PAGE)->prev = prev;
                ((PhysMemoryBlock*)TEMP_PAGE)->size = count;
                qemu_printf("\n(cur_block > base) || (((PhysMemoryBlock*)TEMP_PAGE)->next == free_phys_memory_pointer)");
                break;
            }
            cur_block = ((PhysMemoryBlock*)TEMP_PAGE)->next;

        } while (cur_block != free_phys_memory_pointer);
        
        if (base < free_phys_memory_pointer) {
            free_phys_memory_pointer = base;
        }
    }
    qemu_printf("\nDONE!\n");
    free_page_count += count;
}


/* Virtual memory manager */
int map_pages(phyaddr page_dir, void *vaddr, phyaddr paddr, size_t count, unsigned int flags) {
    for (; count; count--) {
        phyaddr page_table = page_dir;
        char shift;
        for (shift = PHYADDR_BITS - PAGE_TABLE_INDEX_BITS; shift >= PAGE_OFFSET_BITS; shift -= PAGE_TABLE_INDEX_BITS) { 
            unsigned int index = ((size_t)vaddr >> shift) &
            PAGE_TABLE_INDEX_MASK;
            temp_map_page(page_table);
            if (shift > PAGE_OFFSET_BITS) {
                page_table = ((phyaddr*)TEMP_PAGE)[index];
                if (!(page_table & PAGE_VALID)) {
                    phyaddr addr = alloc_phys_pages(1);
                    if (addr != (unsigned int)-1) {
                        temp_map_page(paddr);
                        memset((void*)TEMP_PAGE, 0,
                        PAGE_SIZE);
                        temp_map_page(page_table);
                        ((phyaddr*)TEMP_PAGE)[index] = addr | PAGE_VALID | PAGE_WRITABLE | PAGE_USER;
                        page_table = addr;
                    } else {
                        return 0;
                    }
                }
            } else {
                ((phyaddr*)TEMP_PAGE)[index] = (paddr & ~PAGE_OFFSET_BITS) | flags;
                asm("invlpg (,%0,)"::"a"(vaddr));
            }
        }
        vaddr += PAGE_SIZE;
        paddr += PAGE_SIZE;
    }
    return 1;
}


phyaddr get_page_info(phyaddr page_dir, void *vaddr) {
    phyaddr page_table = page_dir;
    char shift;
    for (shift = PHYADDR_BITS - PAGE_TABLE_INDEX_BITS; shift >= PAGE_OFFSET_BITS; shift -= PAGE_TABLE_INDEX_BITS) {
        unsigned int index = ((size_t)vaddr >> shift) & PAGE_TABLE_INDEX_MASK;
        temp_map_page(page_table);
        if (shift > PAGE_OFFSET_BITS) {
            page_table = ((phyaddr*)TEMP_PAGE)[index];
            if (!(page_table & PAGE_VALID)) {
                return 0;
            }
        } else {
            return ((phyaddr*)TEMP_PAGE)[index];
        }
    }
    return 0;
}



typedef struct {
    phyaddr page_dir;
    void *start;
    void *end;
} AddressSpace;

void *alloc_virt_pages(AddressSpace *as, void *vaddr, phyaddr paddr, size_t count, unsigned int flags);
void free_virt_pages(AddressSpace *as, void *vaddr, size_t count, unsigned int flags);