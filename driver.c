#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "scope.h"

main()
{
	int choice;
	char buffer[32];

	scope_t *top_scope = NULL;
	node_t *tmp = NULL;
	
	fprintf(stderr, "0.search 1.search_all 2.insert 3.new_scope 4.del_scope \n");
	while (1) {
		scanf("%d", &choice);
		switch(choice) {
		case 0:
			scanf("%s", buffer);
			tmp = scope_search(top_scope, buffer);
			if (tmp != NULL) {
				fprintf(stderr, "Found[%s]\n", tmp->name);
			}
			else {
				fprintf(stderr, "Not found[%s]\n", buffer);
			}
			break;

		case 1:
			scanf("%s", buffer);
			tmp = scope_search_all(top_scope, buffer);
			if (tmp != NULL) {
				fprintf(stderr, "Found[%s]\n", tmp->name);
			}
			else {
				fprintf(stderr, "Not found[%s]\n", buffer);
			}
			break;

		case 2:
			scanf("%s", buffer);
			tmp = scope_insert(top_scope, buffer);
			if (tmp != NULL) {
				fprintf(stderr, "Insert[%s]\n", tmp->name);
			}
			else {
				fprintf(stderr, "Cannot Insert[%s]\n", buffer);
			}
			break;

		case 3:
			top_scope = scope_push(top_scope);
			break;

		case 4:
			top_scope = scope_pop(top_scope);
			break;
		}
	}
}


