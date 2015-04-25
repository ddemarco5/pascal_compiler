#ifndef ARGS_H
#define ARGS_H

#include "node.h"
#include "scope.h"

typedef struct arglist_s
{
	int type;
	int found; //treated as a bool
	
	struct arglist_s *next;	
} 
arglist_t;

typedef struct funcstack_s
{
	char* name;
	arglist_t *arglist;
	struct funcstack_s *prev;
}
funcstack_t;

void check_few_args(funcstack_t *stack);
void check_args(funcstack_t *stack, int type);
funcstack_t *init_funcstack();
funcstack_t *write_funcstack(funcstack_t *f, node_t *node);
funcstack_t *push_funcstack(funcstack_t *stack, node_t *node);
funcstack_t *pop_funcstack(funcstack_t *stack);
void print_funcstack(funcstack_t *stack);
#endif

