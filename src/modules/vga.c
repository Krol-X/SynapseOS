// protected mode draw
#include "../include/tty.h"
#include "../include/VGA.h"
unsigned char *pixel = VGA + y*pitch + x*pixelwidth;


void putpixel(unsigned char* screen, int x,int y, int color) {
    unsigned where = x*4 + y*3200;
    screen[where] = color & 255;              // BLUE
    screen[where + 1] = (color >> 8) & 255;   // GREEN
    screen[where + 2] = (color >> 16) & 255;  // RED
}