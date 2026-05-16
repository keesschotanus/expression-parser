#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/parser.h"

size_t convert_unary(char infix[], size_t i);
void convert_symbol(char infix[], size_t i);
size_t add_postfix_operand(char infix[], size_t i);
void add_postfix_operator(char symbol);
bool is_unary(char infix[], size_t i);

int precedence(char symbol)
{
	switch(symbol) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	default:
		return 0;
	}
}

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
	default:
		return 0;
	}
}

// converts infix expression to postfix
void convert(char infix[])
{
	unsigned char symbol;
	push_symbol('#');

	for (size_t i = 0; i < strlen(infix); i++) {
		symbol = infix[i];

		if (isspace(symbol)) continue;

		if (is_unary(infix, i) || (!is_operator(symbol))) {
			i = add_postfix_operand(infix, i);
		} else {
		convert_symbol(infix, i);
		}
	}

	while (peek_symbol() != '#') {
		add_postfix_operator(pop_symbol());
	}

}

bool is_unary(char infix[], size_t i) {
	if (infix[i] == '-' || infix[i] == '+') {

		int prev = (int)i - 1;
		while (prev >= 0 && isspace((unsigned char)infix[prev])) prev--;

		return (prev < 0) || (infix[prev] == '(') || (strchr("+-*/^", infix[prev]) != NULL);
	}

	return false;

}

void convert_symbol(char infix[], size_t i) {
	char symbol = infix[i];

	if (symbol == '(') {
		add_postfix_operator(symbol);
	} else {
		if (symbol == ')') {
			while (peek_symbol() != '(') {
				if (peek_symbol() == '#') {
					fprintf(stderr, "Error: mismatched parentheses in expression\n");
					exit(EXIT_FAILURE);
				}
				add_postfix_operator(pop_symbol());
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
						add_postfix_operator(pop_symbol());
					}
				} else {
					/* Left-associative: pop while stack top has greater or equal precedence */
					while (precedence(symbol) <= precedence(peek_symbol())) {
						add_postfix_operator(pop_symbol());
					}
				}
				push_symbol(symbol);
			}
		}
	}
}

size_t add_postfix_operand(char infix[], size_t i)
{
	char *endptr = NULL;
	double val = strtod(infix + i, &endptr);
	add_postfix_token((Token){ .type = token_operand, .value.operand = val });

	/* update i to the last character of the parsed number */
	if (endptr != NULL && endptr > infix) {
		i = (size_t)(endptr - infix) - 1;
	}

	return i;
}

void add_postfix_operator(char symbol) {
	add_postfix_token((Token){ .type = token_operator, .value.operator = symbol });
}
