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
void register_var(node_t *node);
//regstack_t *pushreg(regstack_t *stack);
//regstack_t *popreg(regstack_t *stack);
//void genprogram(codelist_t *codelist);
void addcode(int instruction);
//void addhead(codelist_t *codelist);

#endif
