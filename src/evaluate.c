#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../include/parser.h"

void evaluate_token(const Token *token)
{
	if (token->type == token_operand) {
		push_operand(token->value.operand);
	} else {
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
	visit_postfix_list(evaluate_token);
	
	return peek_operand();
}

