#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arglist.h"


funcstack_t *init_funcstack(){
	funcstack_t *s = (funcstack_t *)malloc(sizeof(funcstack_t));
	return s;
}

funcstack_t *write_funcstack(funcstack_t *f, char *name){
	f->name = (char *)malloc(sizeof(char)*strlen(name));
	strcpy(f->name, name);
	f->prev = NULL;
	return f;
}

funcstack_t *push_funcstack(funcstack_t *stack, char *name){
	funcstack_t *new = write_funcstack(init_funcstack(),name);
	funcstack_t *x = stack;
	if(stack->name == NULL) memcpy(stack, new, sizeof(funcstack_t));
	while(x->prev != NULL) x = x->prev;
	x->prev = new;
	return new;
}

funcstack_t *pop_funcstack(funcstack_t *stack){
	funcstack_t *x = stack;
	funcstack_t *newtop;
	while(x->prev != NULL){
		newtop = x;
		x = x->prev;
	}
	newtop->prev = NULL;
	return x;
}

void print_funcstack(funcstack_t *stack){
	funcstack_t *x = stack;
	fprintf(stderr, "\n\nFUNCSTACK:");
	//fprintf(stderr, "%s->", x->name);
	while(x->prev != NULL){
		fprintf(stderr, "%s->", x->name);
		x = x->prev;
	}
	if(x->prev == NULL) fprintf(stderr, "Done.");
	fprintf(stderr, "\n\n");
}

/*
node_t *make_node(char *name)
{
	node_t *p = (node_t *)malloc(sizeof(node_t));
	p->name = strdup(name);
	p->returned = 0;
	return p;
}

node_t *node_search(node_t *head, char *name)
{
	node_t *p = head;

	while (p != NULL) {
		if (!strcmp(p->name, name)) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}

node_t *node_insert(node_t *head, char *name)
{
	node_t *p = make_node(name);

	p->next = head;
	return p;
}
*/
