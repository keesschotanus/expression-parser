/**
 * @file postfix_list.c
 * @brief Implements a list for storing tokens in postfix order.
 * @version 0.1
 * @date 2026-05-16
 * 
 * @copyright No copyright, do as you wish as long as you don't blame me for any damage this software may cause.
 */

 #include <stdio.h>
#include <collections/list.h>

#include "parser.h"

static list postfix_list;

void create_postfix_list()
{
        postfix_list = list_create(25, sizeof(Token));
}

void add_postfix_token(Token token)
{
	list_append(postfix_list, &token);
}

void print_postfix_list()
{
	for (size_t i = 0; i < list_size(postfix_list); i++) {
		Token *token = list_get(postfix_list, i);
		if (token->type == token_operand) {
			printf("%f ", token->value.operand);
		} else if (token->type == token_operator) {
			printf("%c ", token->value.operator);
		}
	}
	printf("\n");
}

void visit_postfix_list(void (*visitor)(const Token *))
{
	list_visit(postfix_list, (void (*)(const void *))visitor);
}

void free_postfix_list()
{
	list_free(postfix_list);
}
