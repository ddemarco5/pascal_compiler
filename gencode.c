#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "node.h"
#include "tree.h"
#include "y.tab.h"
#include "gencode.h"

//This will be the head of our asm, we will declare it here.
#define HEAD -1
#define PRINTF -2

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
		fprintf(stderr, "generating code for instr %d\n", iter->type);
		genasm(f, iter->type);
		//fprintf(f, "\n");
		iter = iter->next;
	}
	fclose(f);
}

int is_leaf(tree_t *tree){
	if((tree->right == NULL) && (tree->left == NULL)) return 1;
	else return 0;
}
void gencode_tree(tree_t *tree){

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
					 "\t.size   main, .-main\n"
					 "\t.ident  \"pasc\"\n");
			break;
		case PRINTF:
			fprintf(f, ".LC0:\n"
					 "\t.string \"%%d\"\n"
					 "\t.text\n"
					 "\t.globl  main\n"
					 "\t.type   main, @function\n");
			break;
		case WRITE:
			fprintf(f, "\tmovl    $10, %%esi\n"
					"\tmovl    $.LC0, %%edi\n"
					"\tcall    printf\n");
			break;
		case 11://main open
			fprintf(f, "main:\n"
					".LFB0:\n"  
					"\tpushq   %rbp\n"
					"\tmovq    %rsp, %rbp\n");
			break;
		case 12://main close
			fprintf(f, "\tmovl    $0, %%eax\n"
					"\tpopq    %rbp\n"
					"\tret\n");
			break;

	}
}

void addhead(codelist_t *codelist){
	addcode(codelist, HEAD);
	addcode(codelist, PRINTF);
	//addcode(codelist, 11);
}
