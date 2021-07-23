#include "../lib/stdlib.h"
typedef struct {
    uint64 base;
    uint64 size;
} BootModuleInfo;

void kernel_main(uint8 boot_disk_id, void *memory_map, BootModuleInfo *boot_module_list) {
    char *screen_buffer = (void*)0xB8000;
    memset_word(screen_buffer, 0x0E00, 2000);
    char msg[] = {'H',0x0E,'e',0x0E,'l',0x0E,'l',0x0E,'o',0x0E,' ',0x0E,'w',0x0E,'o',0x0E,'r',0x0E,'l',0x0E,'d',0x0E,'!',0x0E};
    memcpy(screen_buffer, msg, sizeof(msg));
}