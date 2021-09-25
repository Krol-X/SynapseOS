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
	qemu_printf("\ninp = %s", inp);
	
	int size, i, x = 0, y = 0;
	
	char op = '+';
	//Узнаем размер массива
	qemu_printf("\nop = %c", op);
	size = strlen((char *)inp); // strlen посчитает столько там ЕСТЬ символов сейчас
	qemu_printf("\nsize = %d", inp);

	//Парсинг строк
	for (i = 0; i < size; i++){
		qemu_printf("\neval_expr, i = %d", i);
		char c = inp[i];
		// если встретили знак операции
		if (c == '+' || c == '-' || c == '*' || c == '/'){
			// обрабатываем прошлую операцию
			process_op(op, &x, &y);
			op = c; // запоминаем новую операцию
		} else if (c == '\n' || c == '\t' || c == '\a' || c == ' '){
			tty_printf("%s '%c'\n", INVALID_CHARACTER, inp[i]);
		} else {
			y= y*10 + (c - '0');
			tty_printf("y = %d\n", y);
		}
	}
	process_op(op, &x, &y);
	
	return x;
}

void NeraMAth(const char *input_text){
	qemu_printf("\ninput_text = %s", input_text);

	tty_printf("\nResult: %d\n", eval_expr(input_text));
}