#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "node.h"
#include "tree.h"
#include "y.tab.h"

int get_branch_type(tree_t *branch){
	int x;
	switch(branch->type) {
		case ID:
			x = (branch->attribute.sval)->type;
			if( x == INTEGER) return INUM;
			if( x == REAL) return RNUM;
			break;
		case INUM:
			return INUM;
			break;
		case RNUM:
			return RNUM;
			break;
		case ADDOP:
			if(branch->right != NULL)
				return get_branch_type(branch->right);
			break;
		case MULOP:
			if(branch->right != NULL)
				return get_branch_type(branch->right);
			break;
		default:
			fprintf(stderr, "OTHER TYPE (%d)\n", branch->type);
			//exit(1);
			return branch->type;
	}
}

/* constructors */
tree_t *make_tree(int type, tree_t *left, tree_t *right)
{
		  tree_t *p = (tree_t *)malloc(sizeof(tree_t));
		  assert(p != NULL);

		  p->type = type;
		  p->left = left;
		  p->right = right;
		  p->rval = 0;
		  return p;
}

tree_t *make_op(int type, int attribute, tree_t *left, tree_t *right)
{
		  if(get_branch_type(left) != get_branch_type(right)){
			fprintf(stderr, "Type mismatch in tree. %d vs. %d\n", get_branch_type(left), get_branch_type(right));
			exit(1);
		  }
		  tree_t *p = make_tree(type, left, right);
		  p->attribute.opval = attribute;
		  return p;
}

tree_t *make_inum(int val)
{
		  tree_t *p = make_tree(INUM, NULL, NULL);
		  p->attribute.ival = val;
		  return p;
}

tree_t *make_rnum(float val)
{
		  tree_t *p = make_tree(RNUM, NULL, NULL);
		  p->attribute.rval = val;
		  return p;
}

tree_t *make_id(node_t *node)
{
		  tree_t *p = make_tree(ID, NULL, NULL);
		  p->attribute.sval = node;
		  return p;
}



/* preorder */
void print_tree(tree_t *t, int spaces)
{
		  int i;

		  if (t == NULL)
			 return;

		  for (i=0; i<spaces; i++) {
			 fprintf(stderr, " ");
		  }

		  /* process root */
		  switch(t->type) {
			 case ID:
				fprintf(stderr, "[ID:%s]", (t->attribute.sval)->name);
				break;
			 case INUM:
				fprintf(stderr, "[INUM:%d]", t->attribute.ival);
				break;
			 case RNUM:
				fprintf(stderr, "[RNUM:%f]", t->attribute.rval);
				break;
			 case ADDOP:
				fprintf(stderr, "[ADDOP:%d]", t->attribute.opval);
				break;
			 case MULOP:
				fprintf(stderr, "[MULOP:%d]", t->attribute.opval);
				break;
			 case RELOP:
				fprintf(stderr, "[RELOP:%d]", t->attribute.opval);
				break;
			 case ARRAY_ACCESS:
				fprintf(stderr, "[ARRAY_ACCESS:%d]", t->attribute.opval);
				break;
			 case FUNCTION:
				fprintf(stderr, "[FUNCTION:%d]", t->attribute.opval);
				break;
			 default:
				fprintf(stderr, "[UNKNOWN]");
				break;
		  }
		  fprintf(stderr, " - rval:%d", t->rval);
		  fprintf(stderr, "\n");

		  /* go left */
		  print_tree(t->left, spaces+4);
		  /* go right */
		  print_tree(t->right, spaces+4);

}


