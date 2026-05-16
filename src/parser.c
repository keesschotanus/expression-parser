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
size_t add_postfix_operand(char infix[], size_t i);
void add_postfix_operator(char symbol);

// converts infix expression to postfix
void convert(char infix[])
{
	char symbol;
	push_symbol('#');

	for (size_t i = 0; i < strlen(infix); i++) {
		symbol = infix[i];

		if (isspace((unsigned char)symbol)) continue;

		if ((infix[i] == '-' || infix[i] == '+') && (i + 1 < strlen(infix)) &&
				(isdigit((unsigned char)infix[i+1]) || infix[i+1] == '.')) {
			i = convert_unary(infix, i);
			continue;
		}

		if (is_operator(symbol) == 0) {
			if (isdigit((unsigned char)symbol) || symbol == '.') {
				i = add_postfix_operand(infix, i);
			} else {
				add_postfix_operator(symbol);
			}
		} else {
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
	}

	while (peek_symbol() != '#') {
		add_postfix_operator(pop_symbol());
	}

}

size_t convert_unary(char infix[], size_t i) {
	int prev = (int)i - 1;
	while (prev >= 0 && isspace((unsigned char)infix[prev])) prev--;

	bool is_unary = (prev < 0) || (infix[prev] == '(') || (strchr("+-*/^", infix[prev]) != NULL);
	if (is_unary) {
		i = add_postfix_operand(infix, i);
	}

	return i;
}

size_t add_postfix_operand(char infix[], size_t i)
{
	char *endptr = NULL;
	double val = strtod(infix + i, &endptr);
	printf("Parsed unary number: %f\n", val);
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

void evaluate_token(const Token *token)
{
	if (token->type == token_operand) {
		push_operand(token->value.operand);
	} else if (token->type == token_operator) {
		double operand2 = pop_operand();
		double operand1 = pop_operand();
		switch (token->value.operator) {
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
		default:
			fprintf(stderr, "Error: unknown operator '%c'\n", token->value.operator);
			exit(EXIT_FAILURE);
		}
	}
}

double evaluate()
{
	print_postfix_list();
	visit_postfix_list(evaluate_token);
	
	return peek_operand();
}

int main(int argc, char **argv)
{
	create_operand_stack();
	create_symbol_stack();
	create_postfix_list();

	const char *input = (argc > 1) ? argv[1] : "2.1 + 2.2";
	char infix[256];

	/* copy input safely into infix buffer */
	strncpy(infix, input, sizeof(infix) - 1);
	infix[sizeof(infix) - 1] = '\0';

	convert(infix);

	printf("Infix expression is: %s\n", infix);
	// Todo: print postfix expression from the list instead of using a separate buffer
	//	printf("Postfix expression is: %s\n", postfix);
	printf("Evaluated expression is: %f\n", evaluate());

	free_symbol_stack();
	free_operand_stack();
	free_postfix_list();

	return 0;
}