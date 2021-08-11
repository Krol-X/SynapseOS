#include "../include/shell.h"
#include "../include/tty.h"
#include "../include/stdlib.h"
#include "../include/cpu_detect.h"
#include "../include/cmos.h"
#include "../include/memory_manager.h"
#include "../include/kernel.h"
#include "../include/kbd.h"

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
        tty_printf("SynapseOS v%s build %s\n\n", VERSION, __TIMESTAMP__);
        detect_cpu();
        tty_printf("    kernel_page_dir = 0x%x\n", kernel_page_dir);
	    tty_printf("    memory_size = %d MB\n", memory_size / 1024 / 1024);
	    tty_printf("    get_page_info(kernel_page_dir, 0xB8000) = 0x%x\n",get_page_info(kernel_page_dir, (void*)0xB8000));
        tty_printf("    free page count = %d", free_page_count);
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
        tty_printf("                                                                   version: %s", VERSION);
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
        getnowtime();

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

void check_keyboard(){
    if(keycode == 14){
        qemu_printf("Backspase!\n");
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
      if ( keycode   == 42 ) {
        SHIFT = 1;
        qemu_printf("\nSHIFT = %d\n", SHIFT);
        return;
      }
      if ( keycode   == -86 ) {
        SHIFT = 0;
        qemu_printf("\nSHIFT = %d\n", SHIFT);
        return;
      }
      

      if ( keycode  ==  -70 ) {
        if ( CAPS == 0 ){
          CAPS = 1;
        } else {
          CAPS = 0;
        }
        qemu_printf("CAPS = %d", CAPS);
      }
      //tty_printf("\n%d \n", keycode);
      if( keycode  <  0){
        return;
      }

      if (CAPS == 1)	{
        if ( SHIFT == 0 ){
          SHIFT = 1;
        } else {
          SHIFT = 0;
        }
        qemu_printf("SHIFT = %d", SHIFT);
      }

      if(keycode  == ENTER_KEY_CODE) {
        tty_putchar('\n');
        shell_exec(string_mem);

        string_mem_counter  =  0;
        memset(string_mem,  0, STRING_MEM_MAX);

        colors(1);
        tty_printf("\n>");
        colors(0);
        return;
      }
      qemu_printf("SHIFT = %d, CAPS = %d\n", SHIFT, CAPS);
      if ( SHIFT == 0 ){
        tty_putchar(keyboard_map[(unsigned char) keycode]);
      } else {
        qemu_printf("SHIFTED\n");
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