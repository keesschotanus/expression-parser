#include <collections/doubly_linked_list.h>

static dllist operand_stack;

void create_operand_stack()
{
        operand_stack = dllist_create(25, sizeof(double));
}

void push_operand(double operand)
{
	dllist_push(operand_stack, &operand);
}

double pop_operand()
{
	return *(double *)dllist_pop(operand_stack);
}

double peek_operand()
{
	return *(double *)dllist_peek(operand_stack);
}

void free_operand_stack()
{
	dllist_free(operand_stack);
}
