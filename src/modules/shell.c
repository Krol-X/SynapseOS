/*
    Shell module
    Simple console interpritator
*/

#include "../include/shell.h"
#include "../include/tty.h"
#include "../include/stdlib.h"
#include "../include/cpu_detect.h"
#include "../include/cmos.h"
#include "../include/memory_manager.h"
#include "../include/kernel.h"
#include "../include/kbd.h"
#include "../include/vga.h"

// Color theme default = 0
int color_theme = 0;
// Debug default 0
int DEBUG = 0; // Note: Debug works only on Qemu emulatorx


void colors(int element){
    if (color_theme == 0){
        if ( element == 0 ){
            tty_setcolor(VGA_COLOR_LIGHT_CYAN);
        } else if ( element == 1 ) {
            tty_setcolor(VGA_COLOR_LIGHT_GREEN);
        } else if ( element == 2 ) {
            tty_setcolor(VGA_COLOR_WHITE);
        } else{
            tty_setcolor(VGA_COLOR_RED);
        }    
    } else if ( color_theme == 1 ){
        if ( element == 0 ){
            tty_setcolor(VGA_COLOR_LIGHT_YELLOW);
        } else if ( element == 1 ) {
            tty_setcolor(VGA_COLOR_LIGHT_MAGENTA);
        } else{
            tty_setcolor(VGA_COLOR_GREEN);
        }   
    } else{
        if ( element == 0 ){
            tty_setcolor(VGA_COLOR_LIGHT_CYAN);
        } else if ( element == 1 ) {
            tty_setcolor(VGA_COLOR_LIGHT_GREEN);
        } else if ( element == 2 ) {
            tty_setcolor(VGA_COLOR_WHITE);
        } else{
            tty_setcolor(VGA_COLOR_LIGHT_RED);
        }  
    }
}

void shell_exec(char input_command[]){
    colors(2);

    // Help page 1/3
    if( strcmp(input_command, "help") == 0 || strcmp(input_command, "help 1") == 0 ){
        tty_printf("SynapseOS is a free and open source 64x operating system written in FASM and C. Help page 1/3.\nCommands:");
        tty_printf("\n    help <1-3> - info about commands        sysinfo - system information");
        tty_printf("\n    time - info about current time          syscheck - check system health");
        tty_printf("\n    exit - Shutdown SynapseOS               cls or clear - cleaning screen");
        tty_printf("\n    ascii - show all ASCII symbols          debug - enable debug mode");
        tty_printf("\n    colors - Show all color themes          vga test - show all 256 vga colors");
        
    } else if( strcmp(input_command, "help 2") == 0 ){
        // Help page 2/3
        tty_printf("Help page 2/3.\nCommands:");
        tty_printf("\n    hello - info about commands              SID - get system ID");
        tty_printf("\n    version - info about current version     ! - enable safe mode");

    } else if( strcmp(input_command, "help 3") == 0 ){
        // Help page 3/3
        tty_printf("Help page 3/3.\nCommands:");
        tty_printf("\n    ");

    } else if( strcmp(input_command, "syscheck") == 0 ){
        // System health check
        tty_printf("System health: 0");

    } else if( strcmp(input_command, "sysinfo") == 0 ){
        // System info
        tty_printf("SynapseOS v%s build %s\n\n", VERSION, __TIMESTAMP__);
        detect_cpu();
        tty_printf("    kernel_page_dir = 0x%x\n", kernel_page_dir);
	    tty_printf("    memory_size = %d MiB\n", memory_size / 1024 / 1024);
	    tty_printf("    get_page_info(kernel_page_dir, 0xB8000) = 0x%x\n",get_page_info(kernel_page_dir, (void*)0xB8000));
        tty_printf("    free page count = %d\n", free_page_count);
        shell_exec("memory_check");

    } else if( strcmp(input_command, "logo") == 0 ){
        // SynapseOS logo
        colors(0);
	    tty_printf("________________________________________________________________________________\n");
        tty_printf("  #####                                              #######  ##### \n");
        tty_printf(" #     # #   # #    #   ##   #####   ####  ######    #     # #     #\n");
        tty_printf(" #        # #  ##   #  #  #  #    # #      #         #     # #      \n");
        tty_printf("  #####    #   # #  # #    # #    #  ####  #####     #     #  ##### \n");
        tty_printf("       #   #   #  # # ###### #####       # #         #     #       #\n");
        tty_printf(" #     #   #   #   ## #    # #      #    # #         #     # #     #\n");
        tty_printf("  #####    #   #    # #    # #       ####  ######    #######  ##### \n");
        tty_printf("                                                                  version: %s", VERSION);
        tty_printf("________________________________________________________________________________");

    } else if( strcmp(input_command, "vga test") == 0 ){
        // Test all VGA colors
        int i = 0;
        while (i != 256){
            tty_setcolor(i);
            tty_printf("%d ", i);
            putpixel(VGA_GRAFIC_MEMORY, i, i, 1);
            putpixel(VGA_GRAFIC_MEMORY, i+1, i, 2);
            putpixel(VGA_GRAFIC_MEMORY, i+2, i, 3);
            i++;
        }
        colors(0);
        
    } else if( strcmp(input_command, "time") == 0 ){
        // Time from CMOS
        getnowtime();

    } else if( strcmp(input_command, "colors") == 0 ){
        // Colors
        tty_printf("All color themes:\n");
        tty_printf("    theme 0 - default color theme\n    theme 1 - MAGENTA color theme\n");

    } else if( strcmp(input_command, "theme 0") == 0 ){
        // Default color theme
        tty_printf("Color theme: default\n");
        color_theme = 0;

    } else if( strcmp(input_command, "theme 1") == 0 ){
        // Magenta color theme
        tty_printf("Color theme: MAGENTA\n");
        color_theme = 1;

    } else if( strcmp(input_command, "hello") == 0 ){
        // TTY hello world test
        tty_printf("Hello World!");

    } else if( strcmp(input_command, "ignat") == 0 ){
        // Put smile symbol
        tty_printf("%c", (unsigned char)1);

    } else if( strcmp(input_command, "ascii") == 0 ){
        // Show ASCII symbols
        int i = -256;
        while( i != 512 ){
            qemu_printf("\n%d = {%c}", i, (unsigned char)i);
            tty_printf("%c", (unsigned char)i);
            i++;
        }

    } else if( strcmp(input_command, "exit") == 0 ){
        // Shutdown OS
        EXIT = 1;
        qemu_printf("\n SHUTDOWN! \n");

    } else if( strcmp(input_command, "debug") == 0 ){
        // Debug mode
        if ( DEBUG == 0 ){
            DEBUG = 1;
        } else {
            DEBUG = 0;
        }

    } else if( strcmp(input_command, "cls") == 0 || strcmp(input_command, "clear") == 0 ){
        // Cleaning screen
        int i = 0;
        while( i != 25 ){
            tty_printf("\n");
            i++;
        }

        i = 0;
        while( i != 25 ){
            tty_backspace();
            i++;
        }
        
    }else if(strcmp(input_command, "get_memory")==0){
        // Try get memory
        phyaddr arg = alloc_phys_pages(1);
        if (arg != (unsigned int)-1){
            tty_printf("memory gived");
        } else {
            colors(3);
            tty_printf("failed to give memory\n");
            tty_printf("error code: %d", arg);
        }
        
    } else if(strcmp(input_command, "memory_check")==0){
        // Check memory
        colors(2);
        tty_printf("Memory info: \n");
        tty_printf("    free memory: %d\n", get_free_memory_size());
        tty_printf("    free page count: %d", free_page_count);

    } else {
        // Unknown command
        colors(3);
        tty_printf("Unknown command [");
        tty_printf(input_command);
        tty_printf("]");
    }
}


//Check keyboard keycodes
void check_keyboard(){

    // If debug enabled we just save all input keycodes to log
    if ( DEBUG == 1 ){
        qemu_printf("\n%d", keycode);
    }

    // If we have not input, we just close function 
    if ( keyboard_get_input == 0){
        return;
    }

    keyboard_get_input = 0;

    // Keycode 14 is backspase
    if(keycode == 14){
        if ( DEBUG == 1 ){
            qemu_printf("Backspase!\n");
        }
        if (string_mem_counter != 0){
            string_mem_counter--;
            string_mem[ string_mem_counter ] = 0;
            qemu_printf("string_mem = %s    ",string_mem);
            qemu_printf("string_mem_counter = %d    \n",string_mem_counter);
            tty_backspace();
        }
        return;
    }

    if (keycode == -114){
        return;
    }

    if ( keycode == 42 || keycode == 54 ) {
        SHIFT = 1;
        if ( DEBUG == 1 ){
            qemu_printf("\nSHIFT = %d\n", SHIFT);
        }
        return;
    }

    if ( keycode   == -86 ) {
        SHIFT = 0;
        if ( DEBUG == 1 ){
            qemu_printf("\nSHIFT = %d\n", SHIFT);
        }
        return;
    }


    // If caps lock pressed
    if ( keycode  ==  -70 ) {
        if ( CAPS == 0 ){
            CAPS = 1;
        } else {
            CAPS = 0;
        }

        if ( DEBUG == 1 ){
            qemu_printf("CAPS = %d", CAPS);
        }
    }
    if( keycode  ==  75){
        qemu_printf("\nLEFT pressed\n");
        return;
    } else if ( keycode == -32){
        qemu_printf("\nLEFT released\n");
        return;
    }
    if( keycode  <  0){
        return;
    }

    if (CAPS == 1)	{
        if ( SHIFT == 0 ){
            SHIFT = 1;
        } else {
            SHIFT = 0;
        }

        if ( DEBUG == 1 ){
            qemu_printf("\nSHIFT = %d\n", SHIFT);
        }
    }

    if(keycode  == ENTER_KEY_CODE) {
        tty_putchar('\n');
        shell_exec(string_mem);
        string_mem_counter = 0;

        memset(string_mem, 0, STRING_MEM_MAX);

        colors(1);
        tty_printf("\n>");
        colors(0);
        return;
    }

    if ( DEBUG == 1 ){
        qemu_printf("SHIFT = %d, CAPS = %d\n", SHIFT, CAPS);
    }

    if ( SHIFT == 0 ){
        tty_putchar(keyboard_map[(unsigned char) keycode]);
    } else {
        if ( DEBUG == 1 ){
            qemu_printf("SHIFTED\n");
        }
        tty_putchar(keyboard_map_shifted[(unsigned char) keycode]);
    }

    if (string_mem_counter!= STRING_MEM_MAX){
        if (DEBUG==1){
            qemu_printf("\n%d \n", keycode);;
        }
        if (SHIFT == 0){
            string_mem[string_mem_counter]  =  keyboard_map[(unsigned char) keycode];
        } else {
            string_mem[string_mem_counter]  =  keyboard_map_shifted[(unsigned char) keycode];
        }
        string_mem_counter++;
    }  else{
    tty_setcolor(VGA_COLOR_RED);
    tty_printf("\nError:  Buffer is  full.  Buffer cleaned.\n");
    colors(0);

    string_mem_counter  =  0;
    memset( string_mem,  0, STRING_MEM_MAX );
    SHIFT = 0;
    }
}