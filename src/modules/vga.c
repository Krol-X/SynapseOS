// protected mode draw
#include "../include/tty.h"
#include "../include/vga.h"
int pitch = 2;
int pixelwidth = 2;

uint16_t* VGA_GRAFIC_MEMORY = (uint16_t*)0x000A0000;	 //old

void putpixel(uint16_t* screen, int x, int y, int color) {
    unsigned where = x*4 + y*3200;
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
}