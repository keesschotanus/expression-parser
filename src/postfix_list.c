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

void free_postfix_list()
{
	list_free(postfix_list);
}
