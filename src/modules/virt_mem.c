/*
*    SynapseOS
*    Virtual memory manager module
*/

#include "../include/virt_mem.h"
#include "../include/phys_mem.h"
#include "../include/stdlib.h"
#include "../include/tty.h"
#include "../include/qemu_log.h"
#include "../include/memmap.h"


// phisical address of kernel page directory
page_directory *kernel_page_dir;

bool vmm_alloc_page(uintptr_t vaddr) {
	uintptr_t  paddr = pmm_alloc_block();
 	if (!paddr) return false;
	vmm_map_page(paddr, vaddr);
	return true;
}

bool vmm_alloc_page_with_userbit(uintptr_t vaddr) {
	uintptr_t  paddr = pmm_alloc_block();
 	if (!paddr) return false;
	vmm_map_page(paddr, vaddr);
	page_table_entry *pte = GET_PTE(vaddr);
	page_table_entry_add_attrib(pte, I86_PTE_USER);
	return true;
}


void vmm_free_page(uintptr_t vaddr) {
	page_table_entry *pte = GET_PTE(vaddr);
	if (!page_table_entry_is_present(*pte)) {
		tty_printf("oh, you try to delete not present page\n");
		return;
	}
	uintptr_t  block = page_table_entry_frame(*pte);
	if (block) {
  		pmm_free_block(block);
  	}
	page_table_entry_del_attrib(pte, I86_PTE_PRESENT);
}


void vmm_create_kernel_page_dir() {
	kernel_page_dir = (page_directory*)pmm_alloc_block();
	if ((uintptr_t)kernel_page_dir == 0xFFFFFFFF) {
		tty_printf("Failed to allocate phys memory for kernel page dir\n");
		// TODO panic here
		return;
	}
	page_directory *pd = kernel_page_dir;
	memset(pd, 0, sizeof(page_directory));
	for (int i = 0; i < PAGE_ENTRIES; i++) {
		page_dir_entry *pde = (page_dir_entry*)&pd->entries[i];
		page_dir_entry_add_attrib(pde, I86_PTE_WRITABLE);
		page_dir_entry_del_attrib(pde, I86_PTE_PRESENT);
		// we use fractal(recursive) mapping technique, which allows us to access PD and PT
		if (i == PAGE_ENTRIES - 1) {
			page_dir_entry_add_attrib(pde, I86_PTE_PRESENT);
			page_dir_entry_set_frame(pde, (uintptr_t )kernel_page_dir);
			// tty_printf("pd[1023] = %x\n", pd->entries[1023]);
		}
	}
}

void vmm_map_page(uintptr_t  paddr, uintptr_t vaddr) {
	page_dir_entry *pde = GET_PDE(vaddr);
	if (!page_dir_entry_is_present(*pde)) {// if page table isnt present, create it
		uintptr_t  pt_p = pmm_alloc_block();//its phys addr!
		if ((uintptr_t)pt_p == 0xFFFFFFFF) {tty_printf("wtf? no free phys memory\n");return;}
		page_table *pt_v = (page_table*)vmm_temp_map_page(pt_p);//because we need to write !
		memset(pt_v, 0, sizeof(page_table));
		page_dir_entry_add_attrib(pde, I86_PDE_PRESENT);
    	page_dir_entry_add_attrib(pde, I86_PDE_WRITABLE);
    	page_dir_entry_set_frame(pde, pt_p);
	}
	page_table_entry *pte = GET_PTE(vaddr);
	page_table_entry_set_frame(pte, paddr);
	page_table_entry_add_attrib(pte, I86_PTE_PRESENT);
	page_table_entry_add_attrib(pte, I86_PTE_WRITABLE);
	flush_tlb_entry(vaddr);
}

uintptr_t vmm_temp_map_page(uintptr_t  paddr)
{
	page_table_entry *pte = GET_PTE(TEMP_PAGE_ADDR);
	page_table_entry_set_frame(pte, PAGE_ALIGN_DOWN(paddr));//old:DOWN
	page_table_entry_add_attrib(pte, I86_PTE_PRESENT);
	page_table_entry_add_attrib(pte, I86_PTE_WRITABLE);
	// flush_tlb_entry(TEMP_PAGE_ADDR);
	asm volatile("invlpg %0" :: "m" (*(uint32_t *)TEMP_PAGE_ADDR) : "memory" );
	// flush_tlb_all();
	return TEMP_PAGE_ADDR;
}

// switch page directory, reveives physical address
void vmm_switch_page_directory(page_directory *page_dir_phys_addr) {
    asm volatile("mov %0, %%cr3" :: "r"((uint32_t)page_dir_phys_addr));
}

void vmm_init() {
	vmm_create_kernel_page_dir();

	page_table* table1 = (page_table*)pmm_alloc_block();
    page_table* table2 = (page_table*)pmm_alloc_block();

    // Clear allocated page tables
    memset((void*)table1, 0, sizeof(page_table));
    memset((void*)table2, 0, sizeof(page_table));

    // Maps first MB to 3GB
    uintptr_t frame, virt; // frame if physical address of page, virt is virtual
    for (frame = 0x0, virt = 0xC0000000;
		frame < 0x100000;
		frame += PAGE_SIZE, virt += PAGE_SIZE)
	{
        page_table_entry page = 0;
        page_table_entry_add_attrib(&page, I86_PTE_PRESENT);
        page_table_entry_set_frame(&page, frame);
        table1->entries[PAGE_TABLE_INDEX(virt)] = page;
    }
	qemu_printf("vmm: first mb mapped to 3gb\n");

    // Maps kernel pages and phys mem pages
    for (frame = KERNEL_START_PADDR, virt = KERNEL_START_VADDR;
	    frame < KERNEL_PHYS_MAP_END;
		frame += PAGE_SIZE, virt += PAGE_SIZE)
	{
        page_table_entry page = 0;
        page_table_entry_add_attrib(&page, I86_PTE_PRESENT);
        page_table_entry_set_frame(&page, frame);

        table2->entries[PAGE_TABLE_INDEX(virt)] = page;
    }
	qemu_printf("vmm: kernel mapped\n");

    page_dir_entry *pde1 = (page_dir_entry*)&kernel_page_dir->entries[PAGE_DIRECTORY_INDEX(0x00000000)];
    page_dir_entry_add_attrib(pde1, I86_PDE_PRESENT);
    page_dir_entry_add_attrib(pde1, I86_PDE_WRITABLE);
    page_dir_entry_set_frame(pde1, (uintptr_t )table1);

    page_dir_entry *pde2 = (page_dir_entry*)&kernel_page_dir->entries[PAGE_DIRECTORY_INDEX(0xC0100000)];
    page_dir_entry_add_attrib(pde2, I86_PDE_PRESENT);
    page_dir_entry_add_attrib(pde2, I86_PDE_WRITABLE);
    page_dir_entry_set_frame(pde2, (uintptr_t )table2);

	update_phys_memory_bitmap_addr(KERNEL_END_VADDR);

	qemu_printf("vmm: trying enable paging...\n");

	qemu_printf("vmm: enable_paging function address = %x\n", enable_paging);

	enable_paging((uintptr_t )kernel_page_dir);

	tty_printf("vmm: vmm initialized!\n");
}

void vmm_test() {
	tty_printf("kernel_page_dir = %x\n", kernel_page_dir);

	uintptr_t  padr1 = 0xC0500000;
	uintptr_t vadr1 = vmm_temp_map_page(padr1);
	*(uint8_t*)vadr1 = 77;
	tty_printf("%x = %x\n", padr1, *(uint8_t*)vadr1);

	//tty_printf("%x = %x\n", (0x00100000), *(uint8_t*)(0x00100000)); IT WILL CAUSE PAGE FAULT!!!! BEACUSE WE 1:1 MAPPED UP TO 1MB PHYS MEM BUT NEVKLYUCHITELNO!
	tty_printf("%x = %x\n", (0x00100000 - 1), *(uint8_t*)(0x00100000 - 1));
	//asm volatile( "movl %0, %%cr3" :: "r" ( kernel_page_dir ) );

	int eip;
	asm volatile("1: lea 1b, %0;": "=a"(eip));
    tty_printf("EIP = %x  ", eip);
}



void page_table_entry_add_attrib(page_table_entry* entry, uint32_t attrib) {*entry |= attrib;} //add attribute to pte
void page_table_entry_del_attrib(page_table_entry* entry, uint32_t attrib) {*entry &= ~attrib;} //delete attribute to pte
void page_table_entry_set_frame(page_table_entry* entry, uintptr_t addr) {*entry = (*entry & ~I86_PTE_FRAME) | addr;}//map pte to physical frame
bool page_table_entry_is_present(page_table_entry entry) {return entry & I86_PTE_PRESENT;}
bool page_table_entry_is_writable(page_table_entry entry) {return entry & I86_PTE_WRITABLE;}
uintptr_t page_table_entry_frame(page_table_entry entry) {return entry & I86_PTE_FRAME;}//return the address of physical frame which pte refers to

//--------------------------------------------------------------------
//functions for Page Directory Entries

void page_dir_entry_add_attrib(page_dir_entry* entry, uint32_t attrib) {*entry |= attrib;}//add attribute to pde
void page_dir_entry_del_attrib(page_dir_entry* entry, uint32_t attrib) {*entry &= ~attrib;}//old: was without ~ !! //delete attribute to pde
void page_dir_entry_set_frame(page_dir_entry* entry, uintptr_t addr) {*entry = (*entry & ~I86_PDE_FRAME) | addr;}//map pde to physical frame (where the appropriate page table stores)
bool page_dir_entry_is_present(page_dir_entry entry) {return entry & I86_PDE_PRESENT;}
bool page_dir_entry_is_user(page_dir_entry entry) { return entry & I86_PDE_USER; }
bool page_dir_entry_is_4mb(page_dir_entry entry) { return entry & I86_PDE_4MB; }
bool page_dir_entry_is_writable(page_dir_entry entry) {return entry & I86_PDE_WRITABLE;}
uintptr_t page_dir_entry_frame(page_dir_entry entry) {return entry & I86_PDE_FRAME;}//return the address of physical frame which pde refers to

void flush_tlb_entry(uintptr_t addr) { asm volatile("invlpg (%0)" : : "b"(addr) : "memory"); }//???