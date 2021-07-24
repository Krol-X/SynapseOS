#ifndef SCANCODES_H
#define SCANCODES_H
#include "stdlib.h"

char scancodes_shifted[];
char scancodes[];

char in_char(bool wait);
void in_string(char *buffer, size_t buffer_size);


#endif