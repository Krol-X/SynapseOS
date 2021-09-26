#include "../include/tty.h"
#include "../include/NeraMath.h"

void process_op(char op, int *x, int *y) {
	if (op == '+') {
		*x += *y; *y = 0;
	} else if (op == '-') {
		*x -= *y; *y = 0;
	} else if (op == '*') {
		*x *= *y; *y = 0;
	} else if (op == '/') {
		if (*x != 0 && *y != 0) {
			*x /= *y; *y = 0;
		} else {
			tty_printf("%s\n",ZERO_DIVISION);
		}
	}
}

int eval_expr(const char *inp){
	int size, i, x = 0, y = 0;
	
	char op = '+';

	size = strlen((char *)inp); 

	//Парсинг строк
	for (i = 0; i < size; i++){
		char c = inp[i];
		// If char is operator
		if (c == '+' || c == '-' || c == '*' || c == '/'){
			// Execute last operation
			process_op(op, &x, &y);
			op = c; // Save new operation
		} else if (c == '\n' || c == '\t' || c == '\a' || c == ' '){
			tty_printf("%s '%c'\n", INVALID_CHARACTER, inp[i]);
		} else {
			y= y*10 + (c - '0');
			//tty_printf("y = %d\n", y);
		}
	}
	process_op(op, &x, &y);
	
	return x;
}

void NeraMAth(const char *input_text){
	qemu_printf("\ninput_text = %s", input_text);

	tty_printf("\nResult: %d\n", eval_expr(input_text));
}