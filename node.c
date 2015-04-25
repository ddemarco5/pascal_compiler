#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"

node_t *make_node(char *name)
{
	node_t *p = (node_t *)malloc(sizeof(node_t));
	p->args = NULL;
	p->numargs = 0;
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


void add_arg(node_t *node, int type){
	int newsize = node->numargs + 1;
	int *oldarray = node->args;
	int *newarray = (int *)malloc(sizeof(int)*newsize);
	if(node->args != NULL) memcpy(newarray, oldarray, sizeof(int)*(node->numargs));
	node->numargs = node->numargs + 1;
	newarray[node->numargs] = type;
	node->args = newarray;
	free(oldarray);	
}
