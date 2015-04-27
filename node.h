#ifndef NODE_H
#define NODE_H

typedef struct node_s
{
	char *name;
	int type;	/* VAR, FUNCTION, PROCEDURE */
	/* extra information */
	int rtype;
	int returned; //treated as bool

	int offset; //for assembly generation.

	//function/procedure argument vars
	int numargs;

	//an array of args and their types. Will constructed in pc.y
	int *args;
	/* array bounds */
	int astart;
	int aend;

	struct node_s *next;	
} 
node_t;

node_t *make_node(char *name);

/* Add argument to arg list */
void add_arg(node_t *node, int type);

/* Linked List support */
node_t *node_search(node_t *, char *);
node_t *node_insert(node_t *, char *);


#endif

