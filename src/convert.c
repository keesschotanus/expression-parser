/**
 * @file convert.c
 * @brief Converts an infix expression to postfix.
 * 	The conversion is done using the
 *  	[Shunting Yard algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm),
 * 	which uses a stack to handle operator precedence and associativity.
 * 
 * 	The input is a string and the output is a list of tokens in postfix order.
 * @version 0.1
 * @date 2026-05-16
 * 
 * @copyright No copyright, do as you wish as long as you don't blame me for any damage this software may cause.
 */

#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/parser.h"

static void convert_symbol(const char *infix, size_t i);
static size_t add_postfix_operand(const char *infix, size_t i);
static void add_postfix_operator(char symbol);
static bool is_unary(const char *infix, size_t i);

static int precedence(char symbol)
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

static bool is_operator(char symbol)
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
void convert(const char *infix)
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

static bool is_unary(const char *infix, size_t i) {
	if (infix[i] == '-' || infix[i] == '+') {

		int prev = (int)i - 1;
		while (prev >= 0 && isspace((unsigned char)infix[prev])) prev--;

		return (prev < 0) || (infix[prev] == '(') || (strchr("+-*/^", infix[prev]) != NULL);
	}

	return false;

}

static void convert_symbol(const char *infix, size_t i) {
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

static size_t add_postfix_operand(const char *infix, size_t i)
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

static void add_postfix_operator(char symbol) {
	add_postfix_token((Token){ .type = token_operator, .value.operator = symbol });
}
