#ifndef TTY_H
#define TTY_H

#include "vga.h"
#include "ports.h"
#include "qemu_log.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

extern volatile uint8_t tty_feedback;

void tty_init(void);
void tty_setcolor(uint8_t color);
void tty_putentryat(char c, uint8_t color, size_t x, size_t y);
void tty_putchar(char c);
void tty_write(const char* data, size_t size);
void tty_putstring(const char* data);
void tty_putstring_color(const char* data, uint8_t text_color);
void tty_putuint(int i);
void tty_putint(int i);
void tty_puthex(uint32_t i);
void tty_print(char *format, va_list args);
void tty_printf(char *text, ... );
void tty_scroll();
void update_cursor(size_t row, size_t col);


#endif