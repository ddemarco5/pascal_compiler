#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "scope.h"
#include <string.h>

/* instantiate stack of names */
namelist_t *create_namelist(){
	namelist_t *n = (namelist_t*)malloc(sizeof(namelist_t));
	return n;
}

/* write to namelist that has been instantiated */
namelist_t *write_namelist(namelist_t *n, char *name)
{
	n->name = (char*)malloc(sizeof(char*)*strlen(name));
	strcpy(n->name, name);
	n->next = NULL;
	return n;
}
/* clear and delete stack of names*/
void flush_namelist(namelist_t *namelist){
	
	namelist->next = NULL;
	free(namelist->name);
	free(namelist);
	/*namelist_t *n = namelist;
	namelist_t *p;
	while(n->next != NULL){
		p = n;
		n = p->next;*/
		/*set the next to NULL*/
		/*p->next = NULL;
		free(p->name);
		free(p);
	}*/
}
/* insert a name on to the stack */
void insert_name(namelist_t *namelist, char* name){
	namelist_t *n = namelist;
	while(n->next != NULL) n = n->next;
	namelist_t *new = write_namelist(create_namelist(), name);
	/* checks to see if we're at the beginning of the list */
	if(n->name == NULL) write_namelist(namelist, name);
	else n->next = new;
}

/* print the stack */
void print_names(namelist_t *namelist){
	fprintf(stderr, "\nNAMESTACK: ||%s||, ", namelist->name);
	while(namelist->next != NULL){
		namelist = namelist->next;
		fprintf(stderr, " ||%s||,", namelist->name);
	}	
	fprintf(stderr, "\n");
}

/* constructor */
scope_t *make_scope()
{
	int i;
	scope_t *p = (scope_t *)malloc(sizeof(scope_t));

	for (i=0; i<HASH_SIZE; i++) {
		p->table[i] = NULL;
	}
	p->next = NULL;

	return p;
}

/* local search */
node_t *scope_search( scope_t *scope, char *name )
{
	int index;
	node_t *head;

	if (scope != NULL) {
		index = hashpjw(name);
		head = scope->table[index];


		return node_search(head, name);
	}
	else return NULL;
}

/* local insert */
node_t *scope_insert( scope_t *scope, char *name )
{
	int index;
	node_t *head;

	if (scope != NULL) {
		index = hashpjw(name);
		head = scope->table[index];

		return scope->table[index] = node_insert(head, name);
	}
	else return NULL;
}

/* global search */
node_t *scope_search_all( scope_t *curr_scope, char *name )
{
	node_t *p;

	while (curr_scope != NULL) {
		p = scope_search(curr_scope, name);
		if (p != NULL)
			return p;
		curr_scope = curr_scope->next;
	}
	return NULL;
}

/* push new scope */
scope_t *scope_push( scope_t *top )
{

	fprintf(stderr, "\nSCOPE -- PUSHING NEW SCOPE\n");

	scope_t *new_scope = make_scope();
	assert( new_scope != NULL );

	new_scope->next = top;
	return new_scope;
}

/* pop top scope */
scope_t *scope_pop( scope_t *top )
{
	fprintf(stderr, "\nSCOPE -- POPPING SCOPE\n");
	
	scope_t *tmp;

	if (top != NULL) {
		tmp = top;
		top = top->next;

		//free_scope(tmp);

		return top;
	}
	else return NULL;
}


/* Weinberger's magic hash function */
/* page 436 Dragon 1 */
int hashpjw( char *s )
{
	char *p;
	unsigned h = 0, g;

	for ( p=s; *p != EOS; p++ ) {
		h = (h << 4) + (*p);
		if ( g = h&0xf0000000 ) {
			h = h ^ (g >> 24);
			h = h ^ g;
		}
	}
	return h % HASH_SIZE;
}

