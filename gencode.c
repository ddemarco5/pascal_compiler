#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "node.h"
#include "tree.h"
#include "y.tab.h"
#include "gencode.h"

//This will be the head of our asm, we will declare it here.
#define HEAD -1

void genasm(FILE *f, int instr);

void printcodelist(codelist_t *codelist){
	if(codelist == NULL) return;
	fprintf(stderr, "%d,\n", codelist->type);
	printcodelist(codelist->next);
}

void gencode(codelist_t *codelist){
	FILE *f;
	f = fopen("output.s", "w");
	codelist_t *iter = codelist;
	if(iter == NULL) fprintf(stderr, "It's null fag.\n");
	printcodelist(codelist);
	while(iter != NULL){
		fprintf(stderr, "DOING THIS THING!\n");
		genasm(f, iter->type);
		iter = iter->next;
	}
	fclose(f);
}

codelist_t *getend(codelist_t *codelist){
	if(codelist->next == NULL) return codelist;
	getend(codelist->next);
}

void addcode(codelist_t *codelist, int instruction){
	if(codelist->type == -99){
		codelist->type = instruction;
		return;
	}
	codelist_t *end = getend(codelist);
	codelist_t *newcode = (codelist_t *)malloc(sizeof(codelist_t));
	newcode->type = instruction;
	newcode->next = NULL;
	end->next = newcode;
}

void genasm(FILE *f, int instr){
	switch(instr){
		case HEAD:
			fprintf(f, ".LFE0:\n"
					 ".size   main, .-main\n"
					 ".ident  \"pasc\"\n");
	}
}

void addhead(codelist_t *codelist){
	addcode(codelist, HEAD);
}
