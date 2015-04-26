#ifndef G_CODE
#define G_CODE

extern FILE *outfile;
/*
typedef struct codelist_s{
	int type;
	char *arg1;
	char *arg2;
	struct codelist_s *next;
}
codelist_t;
*/

void initfile();
void closefile();
void gentree(tree_t *tree);
//void genprogram(codelist_t *codelist);
//void addcode(codelist_t *codelist, int instruction);
//void addhead(codelist_t *codelist);

#endif
