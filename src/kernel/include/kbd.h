#ifndef KBD_H
#define KBD_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define ENTER_KEY_CODE 0x1C

#define STRING_MEM_MAX 256

extern unsigned char keyboard_map[128];

void kb_init(void);
void keyboard_handler(void);
void keyboard_handler_main(void);

#endif