#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/parser.h"

#define DEBUG 1

int precedence(char symbol)
{
	switch(symbol) {
	case '+':
	case '-':
		return 2;
		break;
	case '*':
	case '/':
		return 3;
		break;
	case '^':
		return 4;
		break;
	case '(':
	case ')':
		return 1;
		break;
	default:
		return 0;
	}
}

// check whether the symbol is operator?
bool is_operator(char symbol)
{

	switch(symbol) {
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
	case '(':
	case ')':
		return 1;
		break;
	default:
		return 0;
	}
}

// converts infix expression to postfix
void convert(char infix[],char postfix[])
{
	size_t i;
	int symbol,j = 0;
	push_symbol('#');

	for (i = 0; i < strlen(infix); i++) {
		symbol = infix[i];

		if (is_operator(symbol) == 0) {
			postfix[j] = symbol;
			j++;
		} else 	{
			if (symbol == '(') {
				push_symbol(symbol);
			} else {
				if (symbol == ')') {
					while (peek_symbol() != '(') {
						if (peek_symbol() == '#') {
							fprintf(stderr, "Error: mismatched parentheses in expression\n");
							exit(EXIT_FAILURE);
						}
						postfix[j] = pop_symbol();
						j++;
					}

					pop_symbol();
				} else {
					/* Handle operator associativity: '^' is right-associative */
					if (precedence(symbol) > precedence(peek_symbol()) ||
						(symbol == '^' && precedence(symbol) == precedence(peek_symbol()))) {
						push_symbol(symbol);
					} else {
						if (symbol == '^') {
							/* Right-associative: pop only while stack top has greater precedence */
							while (precedence(symbol) < precedence(peek_symbol())) {
								postfix[j] = pop_symbol();
								j++;
							}
						} else {
							/* Left-associative: pop while stack top has greater or equal precedence */
							while (precedence(symbol) <= precedence(peek_symbol())) {
								postfix[j] = pop_symbol();
								j++;
							}
						}

						push_symbol(symbol);
					}
				}
			}
		}
	}

	while (peek_symbol() != '#') {
		postfix[j] = pop_symbol();
		j++;
	}

	postfix[j] = '\0';
}

double evaluate(char* postfix)
{
	char ch;
	int i = 0;
	double operand1,operand2;

	while ((ch = postfix[i++]) != '\0') {
		if (DEBUG) printf("ch: %c\n",ch);
		if (isdigit(ch)) {
			push_operand(ch - '0');
			if (DEBUG) printf("pushed: %f\n",(double)ch - '0');
		} else{
			operand2 = pop_operand();
			operand1 = pop_operand();
			if (DEBUG) printf("popped: %f, %f\n",operand1,operand2);
			switch(ch) {
			case '+':
				push_operand(operand1 + operand2);
				break;
			case '-':
				push_operand(operand1 - operand2);
				break;
			case '*':
				push_operand(operand1 * operand2);
				break;
			case '/':
				push_operand(operand1 / operand2);
				break;
			case '^':
				push_operand(pow(operand1, operand2));
				break;
			}
		}
	}

	return peek_operand();
}

int main(void)
{
	create_operand_stack();
	create_symbol_stack();

	char infix[25] = "2^2^3",postfix[25];
	convert(infix,postfix);

	printf("Infix expression is: %s\n",infix);
	printf("Postfix expression is: %s\n",postfix);
	printf("Evaluated expression is: %f\n",evaluate(postfix));

	free_symbol_stack();
	free_operand_stack();

	return 0;
}