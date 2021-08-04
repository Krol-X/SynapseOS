//#include "stdlib.h"
#include "../include/string.h"
#include "../include/tty.h"
#include "../include/memory_manager.h"

typedef struct {
    uint64 base;
    uint64 length;
    uint32 type;
    uint32 acpi_ext_attrs;
} __attribute__((packed)) MemoryMapEntry;


size_t free_page_count = 0;
phyaddr free_phys_memory_pointer = 0;