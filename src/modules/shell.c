/*
	Shell module
	Simple console interpritator
*/

#include "../include/shell.h"
#include "../include/tty.h"
#include "../include/stdlib.h"
#include "../include/cpu_detect.h"
#include "../include/cmos.h"
#include "../include/phys_mem.h"
#include "../include/virt_mem.h"
#include "../include/kernel.h"
#include "../include/kbd.h"
#include "../include/vga.h"

// Color theme default = 0
int color_theme = 0;
// Debug default 0
int DEBUG = 0; // Note: Debug works only on Qemu emulatorx


void colors(int element) {
	if ( element == 0 ) {
		tty_setcolor(VGA_COLOR_LIGHT_CYAN);
	} else if ( element == 1 ) {
		tty_setcolor(VGA_COLOR_LIGHT_GREEN);
	} else if ( element == 2 ) {
		tty_setcolor(VGA_COLOR_WHITE);
	} else {
		tty_setcolor(VGA_COLOR_RED);
	}
}

void shell_exec(char input_command[]) {
	colors(2);

	// Help page 1/3
	if( strcmp(input_command, "help") == 0 || strcmp(input_command, "help 1") == 0 ) {
		tty_printf("SynapseOS is a free and open source 64x operating system written in FASM and C. Help page 1/1.\nCommands:");
		tty_printf("\n   help - info about commands            sysinfo - system information");
		tty_printf("\n   time - info about current time        vga test - show all 256 vga colors");
		tty_printf("\n   exit - Shutdown SynapseOS             cls or clear - cleaning screen");
		tty_printf("\n   ascii - show all ASCII symbols        debug - enable debug mode");
		
	} else if( strcmp(input_command, "sysinfo") == 0 ) {
		// System info
		tty_printf("SynapseOS v%s build %s\n\n", VERSION, __TIMESTAMP__);

		detect_cpu();
		

	} else if( strcmp(input_command, "logo") == 0 ) {
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

	} else if( strcmp(input_command, "vga test") == 0 ) {
		// Test all VGA colors
		int i = 0;
		
		while (i != 256) {
			tty_setcolor(i);
			tty_printf("%d ", i);
			i++;
		}
		colors(0);
		
	} else if( strcmp(input_command, "time") == 0 ) {
		// Time from CMOS
		getnowtime();
		tty_printf("\n");

	} else if( strcmp(input_command, "ascii") == 0 ) {
		// Show ASCII symbols
		int i = -256;
		while( i != 512 ) {
			qemu_printf("\n%d = {%c}", i, (unsigned char)i);
			tty_printf("%c", (unsigned char)i);
			i++;
		}

	} else if( strcmp(input_command, "exit") == 0 ) {
		// Shutdown OS
		EXIT = 1;
		qemu_printf("\n SHUTDOWN! \n");

	} else if( strcmp(input_command, "debug") == 0 ) {
		// Debug mode
		if ( DEBUG == 0 ) {
			DEBUG = 1;
		} else {
			DEBUG = 0;
		}

	} else if( strcmp(input_command, "cls") == 0 || strcmp(input_command, "clear") == 0 ) {
		// Cleaning screen
		int i = 0;
		while( i != 25 ) {
			tty_printf("\n");
			i++;
		}

		i = 0;
		while( i != 25 ) {
			tty_backspace();
			i++;
		}

	} else {
		// Unknown command
		colors(3);
		tty_printf("Unknown command [");
		tty_printf(input_command);
		tty_printf("]");
	}
}


//Check keyboard keycodes
void check_keyboard() {

	// If debug enabled we just save all input keycodes to log
	if ( DEBUG == 1 ) {
		qemu_printf("\n%d", keycode);
	}

	// If we have not input, we just close function 
	if ( keyboard_get_input == 0) {
		return;
	}

	keyboard_get_input = 0;

	// Keycode 14 is backspase
	if(keycode == 14) {
		if ( DEBUG == 1 ) {
			qemu_printf("Backspase!\n");
		}

		if (string_mem_counter != 0) {
			string_mem_counter--;
			string_mem[ string_mem_counter ] = 0;
			qemu_printf("string_mem = %s    ",string_mem);
			qemu_printf("string_mem_counter = %d    \n",string_mem_counter);
			tty_backspace();
		}
		return;
	}

	if (keycode == -114) {
		return;
	}

	if ( keycode == 42 || keycode == 54 ) {
		SHIFT = 1;
		if ( DEBUG == 1 ) {
			qemu_printf("\nSHIFT = %d\n", SHIFT);
		}
		return;
	}

	if ( keycode   == -86 ) {
		SHIFT = 0;

		if ( DEBUG == 1 ) {
			qemu_printf("\nSHIFT = %d\n", SHIFT);
		}
		return;
	}


	// If caps lock pressed
	if ( keycode  ==  -70 ) {
		if ( CAPS == 0 ) {
			CAPS = 1;
		} else {
			CAPS = 0;
		}

		if ( DEBUG == 1 ) {
			qemu_printf("CAPS = %d", CAPS);
		}
	}
	if( keycode  ==  75) {
		qemu_printf("\nLEFT pressed\n");
		return;
	} else if ( keycode == -32) {
		qemu_printf("\nLEFT released\n");
		return;
	}
	if( keycode  <  0) {
		return;
	}

	if (CAPS == 1)	 {
		if ( SHIFT == 0 ) {
			SHIFT = 1;
		} else {
			SHIFT = 0;
		}

		if ( DEBUG == 1 ) {
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

	if ( DEBUG == 1 ) {
		qemu_printf("SHIFT = %d, CAPS = %d\n", SHIFT, CAPS);
	}

	if ( SHIFT == 0 ) {
		tty_putchar(keyboard_map[(unsigned char) keycode]);
	} else {
		if ( DEBUG == 1 ) {
			qemu_printf("SHIFTED\n");
		}
		tty_putchar(keyboard_map_shifted[(unsigned char) keycode]);
	}

	if (string_mem_counter!= STRING_MEM_MAX) {
		if ( DEBUG == 1 ) {
			qemu_printf("\n%d \n", keycode);;
		}

		if (SHIFT == 0) {
			string_mem[string_mem_counter]  =  keyboard_map[(unsigned char) keycode];
		} else {
			string_mem[string_mem_counter]  =  keyboard_map_shifted[(unsigned char) keycode];
		}
		string_mem_counter++;
	} else {
		tty_setcolor(VGA_COLOR_RED);
		tty_printf("\nError:  Buffer is  full.  Buffer cleaned.\n");
		colors(0);

		string_mem_counter  =  0;
		memset( string_mem,  0, STRING_MEM_MAX );
		SHIFT = 0;
	}
}