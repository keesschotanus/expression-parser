/**
 * @file parser.c
 * @brief Converts an infix expression to postfix and evaluates it.
 * @version 0.1
 * @date 2026-05-16
 * 
 * @copyright No copyright, do as you wish as long as you don't blame me for any damage this software may cause.
 */

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

	// In the absence of command-line arguments, use the expression: "+2--3+2^2^3"
	// This expression tests unary operators, operator precedence, and right-associativity of '^'.
	// The expected output is: 2 - -3 + 2^(2^3) = 2 + 3 + 256 = 261
	const char *infix = (argc > 1) ? argv[1] : "+2--3+2^2^3";
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