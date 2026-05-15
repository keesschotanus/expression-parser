#include <collections/doubly_linked_list.h>

static dllist symbol_stack;

void create_symbol_stack()
{
        symbol_stack = dllist_create(25, sizeof(char));
}

void push_symbol(char symbol)
{
	dllist_push(symbol_stack, &symbol);
}

char pop_symbol()
{
	return *(char *)dllist_pop(symbol_stack);
}

char peek_symbol()
{
	return *(char *)dllist_peek(symbol_stack);
}

void free_symbol_stack()
{
	dllist_free(symbol_stack);
}


	
