#ifndef G_CODE
#define G_CODE

extern FILE *outfile;

typedef struct regstack_s{
	char* reg;
	struct regstack_s *prev;
}
regstack_t;


void initfile();
void closefile();
void gentree(tree_t *tree);
//regstack_t *pushreg(regstack_t *stack);
//regstack_t *popreg(regstack_t *stack);
//void genprogram(codelist_t *codelist);
//void addcode(codelist_t *codelist, int instruction);
//void addhead(codelist_t *codelist);

#endif
