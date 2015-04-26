#ifndef G_CODE
#define G_CODE

typedef struct codelist_s{
	int type;

	struct codelist_s *next;
}
codelist_t;

void printcodelist(codelist_t *codelist);
void gentree(tree_t *tree);
void genprogram(codelist_t *codelist);
void addcode(codelist_t *codelist, int instruction);
void addhead(codelist_t *codelist);

#endif
