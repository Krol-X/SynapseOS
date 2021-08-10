#include "../include/shell.h"
#include "../include/tty.h"
#include "../include/stdlib.h"
#include "../include/cpu_detect.h"
#include "../include/cmos.h"
#include "../include/memory_manager.h"

int color_theme = 0;
int DEBUG = 0;


void colors(int element){
    if (color_theme == 0){
        if (element == 0){
            tty_setcolor(VGA_COLOR_LIGHT_CYAN);
        } else{
            tty_setcolor(VGA_COLOR_LIGHT_GREEN);
        }   
    } else if ( color_theme == 1 ){
        if (element == 0){
            tty_setcolor(VGA_COLOR_LIGHT_YELLOW);
        } else{
            tty_setcolor(VGA_COLOR_LIGHT_MAGENTA);
        }   
    } else{
        if (element == 0){
            tty_setcolor(VGA_COLOR_LIGHT_CYAN);
        } else{
            tty_setcolor(VGA_COLOR_LIGHT_GREEN);
        }
    }
}

void shell_exec(char input_command[]){
    //Help
    if( strcmp(input_command, "help") == 0 ){
        tty_setcolor(VGA_COLOR_WHITE);
        tty_printf("SynapseOS is a free and open source 64x operating system written in FASM and C.\nCommands:");
        tty_printf("\n    help - info about commands        sysinfo - system information");
        tty_printf("\n    time - info about current time    hello - Hello World programm");
        tty_printf("\n    exit - Shutdown SynapseOS         cls or clear - cleaning screen");
        tty_printf("\n    ascii - show all ASCII symbols    debug - enable debug mode");
        tty_printf("\n    colors - Show all color themes    vga test - show all 256 vga colors");
        
    } else if( strcmp(input_command, "sysinfo") == 0 ){
        //system info
        tty_setcolor(VGA_COLOR_WHITE);
        tty_printf("SynapseOS v0006 build %s\n\n", __TIMESTAMP__);
        detect_cpu();
        tty_printf("    kernel_page_dir = 0x%x\n", kernel_page_dir);
	    tty_printf("    memory_size = %d MB\n", memory_size / 1024 / 1024);
	    tty_printf("    get_page_info(kernel_page_dir, 0xB8000) = 0x%x\n",get_page_info(kernel_page_dir, (void*)0xB8000));
        tty_printf("    free page count = %d", free_page_count);

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
        tty_printf("                                                                   version: 0006");
        tty_printf("________________________________________________________________________________");

    } else if( strcmp(input_command, "vga test") == 0 ){
        //est all VGA colors
        int i = 0;
        while (i != 256){
            tty_setcolor(i);
            tty_printf("%d ", i);
            i++;
        }
        colors(0);
        
    } else if( strcmp(input_command, "time") == 0 ){
        //Time from CMOS
        read_rtc();
        

    } else if( strcmp(input_command, "colors") == 0 ){
        //Colors
        tty_printf("All color themes:\n");
        tty_printf("    theme_0 - default color theme\n    theme_1 - MAGENTA color theme\n");
        //tty_printf("    theme_2 - blue color theme\n    theme_3 - green color theme\n");

    } else if( strcmp(input_command, "theme_0") == 0 ){
        //Colors
        tty_printf("Color theme: default\n");
        color_theme = 0;

    } else if( strcmp(input_command, "theme_1") == 0 ){
        //Colors
        tty_printf("Color theme: MAGENTA\n");
        color_theme = 1;

    } else if( strcmp(input_command, "hello") == 0 ){
        tty_printf("Hello World!");

    } else if( strcmp(input_command, "ignat") == 0 ){
        tty_printf("%c", (unsigned char)1);

    } else if( strcmp(input_command, "ascii") == 0 ){
        int i = 0;
        while( i != 256 ){
            tty_printf("%c", (unsigned char)i);
            i++;
        }

    } else if( strcmp(input_command, "exit") == 0 ){
        //Exit
        EXIT = 1;

    } else if( strcmp(input_command, "debug") == 0 ){
        //Debug mode
        if ( DEBUG == 0 ){
            DEBUG = 1;
        } else {
            DEBUG = 0;
        }

    } else if( strcmp(input_command, "cls") == 0 || strcmp(input_command, "clear") == 0 ){
        //Cleaning screen
        int i = 0;
        while( i != 25 ){
            tty_printf("\n");
            i++;
        }
<<<<<<< Updated upstream
=======
        
        
    }else if(strcmp(input_command, "get_memory")==0){
        phyaddr arg = alloc_phys_pages(10);
        if (arg != (unsigned int)-1)
        {
            tty_printf("memory gived");
        } else {
            tty_setcolor(VGA_COLOR_RED);
            tty_printf("failed to give memory\n");
            tty_printf("error code: %d", arg);
        }
        
        
    } else if(strcmp(input_command, "memory_check")==0){
        tty_setcolor(VGA_COLOR_GREEN);
        tty_printf("Memory info: \n");
        tty_setcolor(VGA_COLOR_WHITE);
        tty_printf("\tfree memory: %d", get_free_memory_size());
        tty_printf("\n\tfree page count: %d", free_page_count);    
>>>>>>> Stashed changes
    } else if(strcmp(input_command, "")==0){
        //No command

    } else {
        // Unknown command
        tty_setcolor(VGA_COLOR_RED);
        tty_printf("Unknown command [");
        tty_printf(input_command);
        tty_printf("]");

    }
}