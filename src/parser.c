#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/parser.h"


int main(int argc, char **argv)
{
	create_operand_stack();
	create_symbol_stack();
	create_postfix_list();

	const char *input = (argc > 1) ? argv[1] : "+2--3+2^2^3";
	char infix[256];

	/* copy input safely into infix buffer */
	strncpy(infix, input, sizeof(infix) - 1);
	infix[sizeof(infix) - 1] = '\0';

	convert(infix);

	printf("Infix expression is: %s\n", infix);
	printf("Postfix expression is: ");
	print_postfix_list();

	printf("Evaluated expression is: %f\n", evaluate());

	free_symbol_stack();
	free_operand_stack();
	free_postfix_list();

	return 0;
}