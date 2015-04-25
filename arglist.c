#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arglist.h"

arglist_t *init_arglist(int type, int found){
	arglist_t *tmp = (arglist_t *)malloc(sizeof(arglist_t));
	tmp->type = type;
	tmp->found = found;
	tmp->next = NULL;
	return tmp;
}

void push_arglist(funcstack_t *func, int type, int found){
	arglist_t *tmp = init_arglist(type, found);
	tmp->type = type;
	tmp->found = found;
	arglist_t *x = func->arglist;
	arglist_t *previtem = NULL;
	if(func->arglist->type == -1) memcpy(x, tmp, sizeof(arglist_t));
	while(x != NULL){
		previtem = x;
		x = x->next;
	}
	if(previtem != NULL) previtem->next = tmp;
	x = tmp;
}

void build_arglist(funcstack_t *func, node_t *node){
	int i;
	for(i = 0; i < node->numargs; i++){
		push_arglist(func, node->args[i], 0);
	}
}

funcstack_t *init_funcstack(){
	funcstack_t *s = (funcstack_t *)malloc(sizeof(funcstack_t));
	s->name = NULL;
	s->prev = NULL;
	//s->arglist = NULL;
	s->arglist = init_arglist(-1, -1);
	return s;
}

funcstack_t *write_funcstack(funcstack_t *f, node_t *node){
	f->name = (char *)malloc(sizeof(char)*strlen(node->name));
	strcpy(f->name, node->name);
	build_arglist(f, node);
	return f;
}

funcstack_t *push_funcstack(funcstack_t *stack, node_t *node){
	funcstack_t *new = write_funcstack(init_funcstack(), node);
	funcstack_t *x = stack;
	//The if statement handles if it is an empty first element.
	if(stack->name == NULL) memcpy(stack, new, sizeof(funcstack_t));
	else{
		new->prev = stack;
	}
	return new;
}

funcstack_t *pop_funcstack(funcstack_t *stack){
	funcstack_t *x = stack->prev;
	stack->prev = NULL;
	free(stack);
	return x;
}

funcstack_t *top_funcstack(funcstack_t *stack){
	funcstack_t *tmp = stack;
}

void print_arglist(funcstack_t *stack){
	arglist_t *x = stack->arglist;
	fprintf(stderr, "|a|");
	if(x->next == NULL) fprintf(stderr, "(t:%d,f:%d)", x->type, x->found);
	while(x->next != NULL){
		x = x->next;
		fprintf(stderr, "(t:%d,f:%d)", x->type, x->found);
	}
	if(x->next == NULL) fprintf(stderr, "|!a|");
}

void print_funcstack(funcstack_t *stack){
	if(stack == NULL) return;
	funcstack_t *x = stack;
	fprintf(stderr, "\n\nFUNCSTACK:");
	fprintf(stderr, "%s->", x->name);
	print_arglist(x);
	while(x->prev != NULL){
		x = x->prev;
		fprintf(stderr, "%s->", x->name);
		print_arglist(x);
	}
	if(x == NULL) fprintf(stderr, "Done.");
	fprintf(stderr, "\n\n");
}


