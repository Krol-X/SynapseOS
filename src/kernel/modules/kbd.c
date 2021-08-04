/*  PS/2 Keyboard  driver */
#include  "../include/string.h"
#include  "../include/kbd.h"
#include  "../include/ports.h"
#include  "../include/tty.h"
#include  "../include/shell.h"
#include  "../include/qemu_log.h"

char  string_mem[STRING_MEM_MAX];
int string_mem_counter  =  0;
int SHIFT = 0;
int CAPS = 0;

unsigned  char keyboard_map[] = {
  0, 
  27, //`
  '1', '2',  '3', '4',  '5', '6',  '7', '8',	'9',  '0', 
  '-', '=',  
  '\b',	/*  Backspace  */
  '\t',			/*  Tab  */
  'q', 'w',  'e', 'r',	/*  19 */
  't', 'y',  'u', 'i',  'o', 'p',  '[', ']',  '\n',	/*  Enter  key  */
  0,			/*  29 - Control */
  'a', 's',  'd', 'f',  'g', 'h',  'j', 'k',  'l', ';',	/*  39 */
  '\'',  '`', 
  1,		/*  Left shift */
  '\\',  'z', 'x',  'c', 'v',  'b', 'n',			/*  49 */
  'm', ',',  '.', '/',  
  1,				/*  Right  shift  */
  '*',
  0,	/*  Alt  */
  ' ',	/*  Space  bar  */
  -1,	/*  Caps lock  */
  0,	/*  59 - F1  key  ...  >  */
  0, 0,  0, 0,  0, 0,  0, 0,
  0,	/*  <  ...  F10  */
  0,	/*  69 - Num lock*/
  0,	/*  Scroll Lock  */
  0,	/*  Home key */
  0,	/*  Up Arrow */
  0,	/*  Page Up  */
  '-',
  0,	/*  Left Arrow */
  0,
  0,	/*  Right  Arrow  */
  '+',
  0,	/*  79 - End key*/
  0,	/*  Down Arrow */
  0,	/*  Page Down  */
  0,	/*  Insert Key */
  0,	/*  Delete Key */
  0, 0,  0,
  0,	/*  F11  Key  */
  0,	/*  F12  Key  */
  0,	/*  All  other  keys are undefined */
};

unsigned  char keyboard_map_shifted[] =
{
  0, 
  27, //`
  '!', '@',  '#', '$',  '%', '^',  '&', '*',	'(',  ')', 
  '_', '+',  
  '\b',	/*  Backspace  */
  '\t',			/*  Tab  */
  'Q', 'W',  'E', 'R',	/*  19 */
  'T', 'Y',  'U', 'I',  'O', 'P',  '{', '}',  '\n',	/*  Enter  key  */
  0,			/*  29 - Control */
  'A', 'S',  'D', 'F',  'G', 'H',  'J', 'K',  'L', ':',	/*  39 */
  '"',  '`', 
  1,		/*  Left shift */
  '|',  'Z', 'X',  'C', 'V',  'B', 'N',			/*  49 */
  'M', '<',  '>', '?',  
  1,				/*  Right  shift  */
  '*',
  0,	/*  Alt  */
  ' ',	/*  Space  bar  */
  -1,	/*  Caps lock  */
  0,	/*  59 - F1  key  ...  >  */
  0, 0,  0, 0,  0, 0,  0, 0,
  0,	/*  <  ...  F10  */
  0,	/*  69 - Num lock*/
  0,	/*  Scroll Lock  */
  0,	/*  Home key */
  0,	/*  Up Arrow */
  0,	/*  Page Up  */
  '-',
  0,	/*  Left Arrow */
  0,
  0,	/*  Right  Arrow  */
  '+',
  0,	/*  79 - End key*/
  0,	/*  Down Arrow */
  0,	/*  Page Down  */
  0,	/*  Insert Key */
  0,	/*  Delete Key */
  0, 0,  0,
  0,	/*  F11  Key  */
  0,	/*  F12  Key  */
  0,	/*  All  other  keys are undefined */
};

void  kb_init(void)
{
	/*  0xFD is  11111101 - enables only  IRQ1 (keyboard)*/
	outb(0x21 , 0xFD);
}


void  keyboard_handler_main(void)
{
	unsigned  char status;
	char  keycode;

	/*  write  EOI  */
	outb(0x20,  0x20);

	status  =  inb(KEYBOARD_STATUS_PORT);
	/*  Lowest bit of  status will  be set if  buffer is  not  empty  */
	if  (status  &  0x01)  {
		keycode = inb(KEYBOARD_DATA_PORT);
    qemu
    if ( keycode  == 1 ) {
			SHIFT = 1;
			return;
		}
    if ( keycode  ==  -1 ) {
      if ( CAPS == 0 ){
        CAPS = 1;
      } else {
        CAPS = 0;
      }
    }
		if(keycode  <  0){
      return;
    }

    if (CAPS == 1)	{
      if ( SHIFT == 0 ){
        SHIFT = 1;
      } else {
        SHIFT = 0;
      }
      
    }

		if(keycode  == ENTER_KEY_CODE) {
			tty_putchar('\n');
      shell_exec(string_mem);

      string_mem_counter  =  0;
      memset(string_mem,  0, STRING_MEM_MAX);

      tty_setcolor(VGA_COLOR_LIGHT_GREEN);
      tty_printf("\n>");
      tty_setcolor(VGA_COLOR_LIGHT_CYAN);
			return;
		}
    
    if ( SHIFT == 0 ){
      tty_putchar(keyboard_map[(unsigned char) keycode]);
    } else {
      tty_putchar(keyboard_map_shifted[(unsigned char) keycode]);
    }
    tty_putchar(keyboard_map[(unsigned char) keycode]);
    if (string_mem_counter!= STRING_MEM_MAX){
      if (DEBUG==1){
        tty_printf("\n%c\n",(unsigned char) keycode);
      }
      
    string_mem[string_mem_counter]  =  keyboard_map[(unsigned char) keycode];
    string_mem_counter++;
    }  else{
    tty_setcolor(VGA_COLOR_RED);
    tty_printf("\nError:  Buffer is  full.  Buffer cleaned.\n");
    string_mem_counter  =  0;
    memset(string_mem,  0, STRING_MEM_MAX);
    }

  //vidptr[current_loc++]  =  keyboard_map[(unsigned char) keycode];
		//vidptr[current_loc++] = 0x07;
	}
}

