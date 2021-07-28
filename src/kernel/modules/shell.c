#include "shell.h"
#include "tty.h"
#include "string.h"
#include "cpu_detect.h"

void shell_exec(char input_command[]){
    //Help
    if(strcmp(input_command, "help")==0){
        tty_setcolor(VGA_COLOR_WHITE);
        tty_printf("\nSynapseOS is a free and open source 64x operating system written in FASM and C.\nCommands:\n    help - info about commands\n    sysinfo - system information");
        
    } else if(strcmp(input_command, "sysinfo")==0){
        //system info
        tty_setcolor(VGA_COLOR_WHITE);
        tty_printf("\nSynapseOS v0005 build %s\n\n", __TIMESTAMP__);
        tty_setcolor(VGA_COLOR_LIGHT_GREY);
        detect_cpu();

    } else if(strcmp(input_command, "logo")==0){
        // SynapseOS logo
        tty_setcolor(VGA_COLOR_LIGHT_CYAN);
	    tty_printf("________________________________________________________________________________");
        tty_printf("  #####                                              #######  #####  \n");
        tty_printf(" #     # #   # #    #   ##   #####   ####  ######    #     # #     # \n");
        tty_printf(" #        # #  ##   #  #  #  #    # #      #         #     # #       \n");
        tty_printf("  #####    #   # #  # #    # #    #  ####  #####     #     #  #####  \n");
        tty_printf("       #   #   #  # # ###### #####       # #         #     #       # \n");
        tty_printf(" #     #   #   #   ## #    # #      #    # #         #     # #     # \n");
        tty_printf("  #####    #   #    # #    # #       ####  ######    #######  #####  \n");
        tty_printf("                                                                   version: 0005");
        tty_printf("________________________________________________________________________________");

    } else if(strcmp(input_command, "")==0){
        //No command

    } else {
        // No command
        tty_setcolor(VGA_COLOR_LIGHT_RED);
        tty_printf("\nUnknown command [");
        tty_printf(input_command);
        tty_printf("]");

    }
}