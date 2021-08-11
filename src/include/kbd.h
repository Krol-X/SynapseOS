#ifndef KBD_H
#define KBD_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define ENTER_KEY_CODE 0x1C

#define STRING_MEM_MAX 64

extern unsigned char keyboard_map[128];
extern unsigned char keyboard_map_shifted[128];
extern char  string_mem[STRING_MEM_MAX];
extern int string_mem_counter;
extern int SHIFT;
extern int CAPS;
extern unsigned char  status;
extern char keycode;


void kb_init(void);
void keyboard_handler(void);
char keyboard_handler_main(void);

#endif