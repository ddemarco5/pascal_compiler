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
#define MAXREG 4

FILE *outfile;

void initfile(){
	outfile = fopen("output.s", "w");
}

void closefile(){
	fclose(outfile);
}

void genasm(FILE *f, int instr);

/* void genprogram(){
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
*/

int is_leaf(tree_t *tree){
	if((tree->right == NULL) && (tree->left == NULL)) return 1;
	else return 0;
}
void gentree(tree_t *tree){
	if((tree->left == NULL) && (tree->right == NULL)) return;
	if(is_leaf(tree->left) != 1) gentree(tree->left);
	if(is_leaf(tree->right) != 1) gentree(tree->right);

	if(is_leaf(tree->left) == 1) tree->left->rval = 1;


	if(tree->right->rval == tree->left->rval){
		tree->rval=tree->right->rval+tree->left->rval;
		return;
	}
	else{
		int a,b;
		a = tree->right->rval;
		b = tree->left->rval;
		tree->rval = a > b ? a : b; 
	}
}

void addcode(int instruction){
	genasm(outfile, instruction);
}

addhead(){
	addcode(HEAD);
	addcode(PRINTF);
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

void gencode_helper(tree_t *tree, tree_t *prev){
	if((tree->left == NULL) && (tree->right == NULL)){
		fprintf(stderr, "CASE 0\n");
		return;
	}
	if((is_leaf(tree) == 1) && (prev->left == tree)){
		fprintf(stderr, "CASE 0\n");
	}
	else if(is_leaf(tree->right) == 1){
		fprintf(stderr, "CASE 1\n");
		gencode_helper(tree->left, tree);
	}
	else if(tree->right->rval > tree->left->rval){
		fprintf(stderr, "CASE 2\n");
		gencode_helper(tree->right, tree);
		gencode_helper(tree->left, tree);
	}
	else if(tree->left->rval >= tree->right->rval){
		fprintf(stderr, "CASE 3\n");
		gencode_helper(tree->left, tree);
		gencode_helper(tree->right, tree);
	}
	else if((tree->left->rval > MAXREG) && (tree->right->rval > MAXREG)){
		fprintf(stderr, "CASE 4\n");
	}
	else fprintf(stderr, "NUTHIN!\n");

}

// For now print of the rules triggered
void gencode(tree_t *tree){
	gencode_helper(tree, tree);	
}
