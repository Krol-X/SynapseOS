#include "shell.h"
#include "tty.h"
#include "string.h"

void shell_exec(char *input_command){
    char *help= "help";
    if(strcmp(input_command, help)==0){
        tty_printf("\nSynapseOS is a free and open source 64x operating system written in FASM and C.\nCommands:\n    help - info about commands\n    sysinfo - system information");
    } else{
        tty_printf("\n");
        tty_printf("[");
        tty_printf(input_command);
        tty_printf("]");
    }
}