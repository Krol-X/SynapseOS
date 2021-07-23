#include "../lib/stdlib.h"
#include "../lib/tty.h"
#include "../lib/interrupts.h"

typedef struct {
    uint64 base;
    uint64 size;
} BootModuleInfo;

void kernel_main(uint8 boot_disk_id, void *memory_map, BootModuleInfo *boot_module_list) {
    init_interrupts();
    init_tty();
    
    set_text_attr(0x1F);
    clear_screen();

    out_string("***SynapseOS 0001***\n\n");
    printf("Boot disk id is %d\n", boot_disk_id);
    printf("Memory map at 0x%x\n", memory_map);
    printf("Boot module list at 0x%x\n", boot_module_list);
    printf("String is %s, char is %c, number is %d, hex number is 0x%x\n\n",
        __DATE__, 'A', 1234, 0x1234);
    printf("Enter help to get information about commands\n");

}