#include "../lib/stdlib.h"
#include "../lib/interrupts.h"
#include "../lib/tty.h"

#define OS_VERSION "0002"

typedef struct {
    uint64 base;
    uint64 size;
} BootModuleInfo;

void kernel_main(uint8 boot_disk_id, void *memory_map, BootModuleInfo *boot_module_list) {

    init_interrupts();
    init_tty();

    set_text_attr(11);
    clear_screen();

    printf("********************************SynapseOS %sv********************************\n\n", OS_VERSION);

    set_text_attr(10);
    printf("    Boot disk id is %d\n", boot_disk_id);
    printf("    Memory map at 0x%x\n", memory_map);
    printf("    Boot module list at 0x%x\n", boot_module_list);
    //printf("String is %s, char is %c, number is %d, hex number is 0x%x\n\n", __DATE__, 'A', 1234, 0x1234);
    printf("    DATE is %s\n\n", __DATE__);
    out_string("Enter help to get information about commands\n\n");

    set_text_attr(11);
    out_string("********************************************************************************\n\n");

    set_text_attr(7);

    char buffer[1];
    //i = 1 / 0;
    while (true) {
        
        out_string("Enter string: ");
        in_string(buffer, sizeof(buffer));
        
        printf("You typed: %s\n", buffer);
    }
    printf("You typed: %s\n", buffer);
}