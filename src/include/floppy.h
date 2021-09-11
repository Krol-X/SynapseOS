#include "../include/ports.h"
#include "../include/tty.h"
#include "../include/time.h"
#include "../include/stdlib.h"

void floppy_detect_drives();
void floppy_write_cmd(int base, char cmd);
char *floppy_read_data(int base);