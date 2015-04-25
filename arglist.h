#ifndef ARGS_H
#define ARGS_H

#include "node.h"

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
	struct funcstack_s *prev;
}
funcstack_t;

funcstack_t *init_funcstack();
funcstack_t *write_funcstack(funcstack_t *f, char *name);
funcstack_t *push_funcstack(funcstack_t *stack, char *name);
funcstack_t *pop_funcstack(funcstack_t *stack);
void print_funcstack(funcstack_t *stack);
#endif

