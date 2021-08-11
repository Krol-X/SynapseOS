#ifndef _QEMU_LOG_H
#define _QEMU_LOG_H

#include "ports.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

void qemu_puthex(uint32_t i);
void qemu_printf(char *text, ... );

#endif