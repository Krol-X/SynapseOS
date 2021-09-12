#ifndef _MEMMAP_H_
#define _MEMMAP_H_

#include <stdint.h>

// Constants to the boundaries of memory
#define KERNEL_START_PADDR ((void*)&kernel_phys_start)
#define KERNEL_END_PADDR ((void*)&kernel_phys_end)
#define KERNEL_SIZE (KERNEL_END_PADDR - KERNEL_START_PADDR)

extern uint32_t kernel_phys_start;
extern uint32_t kernel_phys_end;

#endif