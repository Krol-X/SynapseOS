#include "../include/shell.h"
#include "../include/tty.h"
#include "../include/string.h"
#include "../include/cpu_detect.h"
#include "../include/cmos.h"

void shell_exec(char input_command[]){
    //Help
    if(strcmp(input_command, "help")==0){
        tty_setcolor(VGA_COLOR_WHITE);
        tty_printf("SynapseOS is a free and open source 64x operating system written in FASM and C.\nCommands:");
        tty_printf("\n    help - info about commands\n    sysinfo - system information");
        tty_printf("\n    time - info about current time\n    hello - Hello World programm");
        
    } else if(strcmp(input_command, "sysinfo")==0){
        //system info
        tty_setcolor(VGA_COLOR_WHITE);
        tty_printf("SynapseOS v0006 build %s\n\n", __TIMESTAMP__);
        detect_cpu();

    } else if(strcmp(input_command, "logo")==0){
        // SynapseOS logo
        tty_setcolor(VGA_COLOR_LIGHT_CYAN);
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

    } else if(strcmp(input_command, "time")==0){
        //Time
        read_rtc();
        

    } else if(strcmp(input_command, "hello")==0){
        tty_printf("Hello World!");

    } else if(strcmp(input_command, "no-logo")==0){
        //No logo
        tty_printf("________________________________________________________________________________\n");
        tty_printf("SynapseOS");
        tty_printf("                                                                   version: 0006");
        tty_printf("________________________________________________________________________________");

    } else if(strcmp(input_command, "")==0){
        //No command

    } else {
        // Unknown command
        tty_setcolor(VGA_COLOR_LIGHT_RED);
        tty_printf("Unknown command [");
        tty_printf(input_command);
        tty_printf("]");

    }
}