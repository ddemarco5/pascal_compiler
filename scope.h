#ifndef SCOPE_H
#define SCOPE_H

#include "node.h"

#define HASH_SIZE	211
#define	EOS			'\0'

typedef struct scope_s {
	/* hash table: hashing with chaining */
	node_t *table[HASH_SIZE];

	struct scope_s *next;
}
scope_t;

/* list of names to add types to */
typedef struct namelist_s {
	char* name;
	struct namelist_s *next;
} namelist_t;

/* Name list functions */
namelist_t *create_namelist();
namelist_t *write_namelist(namelist_t *n, char *name);
void typify_namelist(scope_t *scope, namelist_t *namelist, int type);
void flush_namelist(namelist_t *namelist);
void insert_name(namelist_t *namelist, char* name);
void print_names(namelist_t *namelist);

/* constructor */
scope_t *make_scope();


/* local search and insert */
node_t *scope_search( scope_t *head, char *name );
node_t *scope_insert( scope_t *head, char *name );

node_t *scope_search_all( scope_t *head, char *name );

/* symbol table = stack of hash tables */
scope_t *scope_push(scope_t *);
scope_t *scope_pop(scope_t *);



#endif


