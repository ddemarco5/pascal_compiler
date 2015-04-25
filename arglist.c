#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arglist.h"

void print_arglist(arglist_t *arglist){
	if(arglist == NULL) return;
	fprintf(stderr, "(t:%d,f:%d)", arglist->type, arglist->found);
	print_arglist(arglist->next);
}

void print_funcstack(funcstack_t *stack){
	if(stack == NULL){ fprintf(stderr, "\n\n"); return; }
	fprintf(stderr, "%s->", stack->name);
	print_arglist(stack->arglist);
	print_funcstack(stack->prev);
}

void check_few_args(funcstack_t *stack){
	arglist_t *list = stack->arglist;
	while((list->found == 1)){
		list = list->next;
		if(list == NULL) break;
	}
	if(list != NULL){
		fprintf(stderr, "Too few arguments provided.\n");
		exit(1);
	}
}

void check_args(funcstack_t *stack, int type){
	int args_tested = 1;
	arglist_t *arglist = stack->arglist;

	// Go to the closest unfound item.
	while(arglist->found == 1){
		fprintf(stderr, "Skipping %d...\n", arglist->type);
		if(arglist->next == NULL){
			fprintf(stderr, "Too many arguments provided in function/procedure.\n");
			exit(1);
		}
		arglist = arglist->next;
		args_tested++;
	}
	fprintf(stderr, "ARGS TESTED: %d.\n", args_tested);
	// Check to see if the types match.
	fprintf(stderr, "DEBUG ARG TYPES: %d, %d.\n", arglist->type, type);
	
		
	if(arglist->type == type){
		fprintf(stderr, "ARGUMENT SATISFIED, SETTING!\n");
		arglist->found = 1;
		return;
	}
	if(arglist->type != type){
		fprintf(stderr, "Invalide argument in function/procedure.\n");
		exit(1);
	}
	else{
		fprintf(stderr, "SOME WEIRD SHIT HAPPENED...\n");
		exit(1);
	}
}

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
	if(func->arglist == NULL){
		func->arglist = tmp;
		return;
	}
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
	s->arglist = NULL;
	//s->arglist = init_arglist(-1, -1);
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


