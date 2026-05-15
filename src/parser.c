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

size_t convert_unary(char infix[], size_t i);

// converts infix expression to postfix
void convert(char infix[],char postfix[])
{
	char symbol;
	int j = 0;
	push_symbol('#');

	for (size_t i = 0; i < strlen(infix); i++) {
		symbol = infix[i];

		/* skip whitespace in input */
		if (isspace((unsigned char)symbol)) continue;

		if ((infix[i] == '-' || infix[i] == '+') && (i + 1 < strlen(infix)) &&
				(isdigit((unsigned char)infix[i+1]) || infix[i+1] == '.')) {
			i = convert_unary(infix, i);
			continue;
		}

		if (is_operator(symbol) == 0) {
			/* Emit multi-digit and decimal numbers (and other non-operator tokens) as space-separated tokens */
			if (isdigit((unsigned char)symbol) || symbol == '.') {
				size_t k = i;
				while (k < strlen(infix) && (isdigit((unsigned char)infix[k]) || infix[k] == '.')) {
					postfix[j++] = infix[k++];
				}
				postfix[j++] = ' ';
				i = k - 1; /* for-loop will increment i */
			} else {
				postfix[j++] = symbol;
				postfix[j++] = ' ';
			}
		} else {
			if (symbol == '(') {
				push_symbol(symbol);
			} else {
				if (symbol == ')') {
					while (peek_symbol() != '(') {
						if (peek_symbol() == '#') {
							fprintf(stderr, "Error: mismatched parentheses in expression\n");
							exit(EXIT_FAILURE);
						}
						postfix[j++] = pop_symbol();
						postfix[j++] = ' ';
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
								postfix[j++] = pop_symbol();
								postfix[j++] = ' ';
							}
						} else {
							/* Left-associative: pop while stack top has greater or equal precedence */
							while (precedence(symbol) <= precedence(peek_symbol())) {
								postfix[j++] = pop_symbol();
								postfix[j++] = ' ';
							}
						}

						push_symbol(symbol);
					}
				}
			}
		}
	}

	while (peek_symbol() != '#') {
		postfix[j++] = pop_symbol();
		postfix[j++] = ' ';
	}

	postfix[j] = '\0';
}

size_t convert_unary(char infix[], size_t i) {
	int prev = (int)i - 1;
	while (prev >= 0 && isspace((unsigned char)infix[prev])) prev--;

	bool is_unary = (prev < 0) || (infix[prev] == '(') || (strchr("+-*/^", infix[prev]) != NULL);
	if (is_unary) {
		/* start of number (including sign) */
		size_t start = i;
		if (infix[start] == '+' || infix[start] == '-') start++;

		char *endptr = NULL;
		double val = strtod(infix + i, &endptr);
		printf("Parsed unary number: %f\n", val);
		add_postfix_token((Token){ .type = token_operand, .value.operand = val });

		/* update i to the last character of the parsed number */
		if (endptr != NULL && endptr > infix) {
			i = (size_t)(endptr - infix) - 1;
		}
	}

	return i;
}

double evaluate(char* postfix)
{
	double operand1;
	double operand2;
	char *p = postfix;
	char *endptr;

	while (*p != '\0') {
		while (*p != '\0' && isspace((unsigned char)*p)) p++;
		if (*p == '\0') break;

		if (isdigit((unsigned char)*p) || *p == '.' ||
			((*p == '+' || *p == '-') && (isdigit((unsigned char)*(p+1)) || *(p+1) == '.'))) {
			double val = strtod(p, &endptr);
			push_operand(val);
			if (DEBUG) printf("pushed: %f\n", val);
			p = endptr;
		} else {
			char op = *p++;
			operand2 = pop_operand();
			operand1 = pop_operand();
			if (DEBUG) printf("popped: %f, %f\n", operand1, operand2);
			switch (op) {
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

int main(int argc, char **argv)
{
	create_operand_stack();
	create_symbol_stack();
	create_postfix_list();

	const char *input = (argc > 1) ? argv[1] : "2.1 + 2.2";
	char infix[256];
	char postfix[512];

	/* copy input safely into infix buffer */
	strncpy(infix, input, sizeof(infix) - 1);
	infix[sizeof(infix) - 1] = '\0';

	convert(infix, postfix);

	printf("Infix expression is: %s\n", infix);
	printf("Postfix expression is: %s\n", postfix);
	printf("Evaluated expression is: %f\n", evaluate(postfix));

	free_symbol_stack();
	free_operand_stack();
	free_postfix_list();

	return 0;
}