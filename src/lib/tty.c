#include "stdlib.h"
#include "tty.h"

typedef struct {
    uint8 chr;
    uint8 attr;
} TtyChar;


unsigned int tty_width;
unsigned int tty_height;
unsigned int cursor;
uint8 text_attr;
TtyChar *tty_buffer;
uint16 tty_io_port;


void init_tty() {
    tty_buffer = (void*)0xB8000;
    tty_width = *((uint16*)0x44A);
    tty_height = 25;
    tty_io_port = *((uint16*)0x463);
    cursor = (*((uint8*)0x451)) * tty_width + (*((uint8*)0x450));
    text_attr = 7;
}


void out_char(char chr) {
    switch (chr) {
        case '\n':
            move_cursor((cursor / tty_width + 1) * tty_width);
            break;
        default:
            tty_buffer[cursor].chr = chr;
            tty_buffer[cursor].attr = text_attr;
            move_cursor(cursor + 1);
    }
}


void out_string(char *str) {
    while (*str) {
        out_char(*str);
        str++;
    }
}


void clear_screen() {
    memset_word(tty_buffer, (text_attr << 8) + ' ', tty_width * tty_height);
    move_cursor(0);
}

void set_text_attr(char attr) {
    text_attr = attr;
}


void move_cursor(unsigned int pos) {
    cursor = pos;
    if (cursor >= tty_width * tty_height) {
        cursor = (tty_height - 1) * tty_width;
        memcpy(tty_buffer, tty_buffer + tty_width, tty_width * tty_height
            * sizeof(TtyChar));
        memset_word(tty_buffer + tty_width * (tty_height - 1), (text_attr << 8) + ' ', tty_width);
    }
    asm("movw %w0, %%dx \n movl %1, %%ecx \n movb $0x0E, %%al \n movb %%ch, %%ah \n outw %%ax, %%dx \n incb %%al \n movb %%cl, %%ah \n outw %%ax, %%dx"::"d"(tty_io_port),"c"(cursor));
}