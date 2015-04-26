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
#define MAXREG 2
//Declaring our R's for the register stack.
#define R0 "%edx"
#define R1 "%ebx"

FILE *outfile;

regstack_t *regstack;

void genasm(FILE *f, tree_t *tree, char* arg1, char* arg2);

void initfile(){
	outfile = fopen("output.s", "w");
}

void closefile(){
	fclose(outfile);
}

void *pushreg(char* reg){
	regstack_t *newreg = (regstack_t *)malloc(sizeof(regstack_t));
	newreg->prev = regstack;
	newreg->reg = reg;
	regstack = newreg;
	//return newreg;
}

regstack_t *popreg(){
	regstack_t *toreturn = regstack;
	regstack = toreturn->prev;
	toreturn->prev = NULL;
	return toreturn;
}

void *swapreg(){
	regstack_t *first = popreg();
	regstack_t *second = popreg();
	pushreg(second->reg);
	pushreg(first->reg);
}

void *regflush(regstack_t *stack){
	if(stack == NULL) return;
	regflush(stack->prev);
	free(stack);
	stack = NULL;
}


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
	genasm(outfile, make_tree(instruction, NULL, NULL), NULL, NULL);
}

addhead(){
	addcode(HEAD);
	addcode(PRINTF);
}

void geninstr(FILE *f, char* instr, char* arg1, char* arg2){	
	fprintf(f, "\t%s \t%s, %s\n", instr, arg1, arg2);
}

// Our non-dynamic assembly bits.
void genasm(FILE *f, tree_t *tree, char* arg1, char* arg2){
	switch(tree->type){
		case ADDOP:
			if(tree->attribute.opval == PLUS)
				fprintf(f, "\taddl \t%s, %s\n", arg1, arg2);
			else if(tree->attribute.opval == MINUS)
				fprintf(f, "\tsubl \t%s, %s\n", arg1, arg2);
			break;
		case MULOP:
			if(tree->attribute.opval == STAR)
				fprintf(f, "\timull \t%s, %s\n", arg1, arg2);
			else if(tree->attribute.opval == SLASH)
				fprintf(f, "\timull \t%s, %s\n", arg1, arg2);
			break;
		case HEAD:
			fprintf(f, ".LFE0:\n"
					 "\t.size   main, .-main\n"
					 "\t.ident  \"pasc\"\n");
			break;
		case PRINTF:
			fprintf(f, ".LC0:\n"
					 "\t.string \"%%d\\n\"\n"
					 "\t.text\n"
					 "\t.globl  main\n"
					 "\t.type   main, @function\n");
			break;
		case WRITE:
			fprintf(f, "\tmovl    %%edx, %%esi\n"
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

char *gettreeval(tree_t *tree){
	char *buf = (char *)malloc(sizeof(int)*3+2);
	switch(tree->type){
		case INUM:
			snprintf(buf, sizeof(buf), "$%d", tree->attribute.ival);
			break;
		case RNUM:
			snprintf(buf, sizeof(buf), "$%d", tree->attribute.rval);
			break;
		}
	return buf;
}

void gencode_helper(tree_t *tree, tree_t *prev){
	if((tree->left == NULL) && (tree->right == NULL)){
		fprintf(stderr, "CASE (NULL) 0\n");
		geninstr(outfile, "movl", gettreeval(tree) , regstack->reg);
		return;
	}
	/*else if((is_leaf(tree) == 1) && (prev->left == tree)){
		fprintf(stderr, "CASE 0\n");
		char buf[sizeof(int)*3+2];
		switch(tree->type){
		case INUM:
			snprintf(buf, sizeof(buf), "%d", tree->attribute.ival);
			break;
		case RNUM:
			snprintf(buf, sizeof(buf), "%d", tree->attribute.rval);
			break;
		}
		geninstr(outfile, "movl", buf , regstack->reg);
		return;

	}*/
	else if(is_leaf(tree->right) == 1){
		fprintf(stderr, "CASE 1\n");
		gencode_helper(tree->left, tree);
		genasm(outfile, tree, gettreeval(tree->right), regstack->reg); 
	}
	else if(tree->right->rval > tree->left->rval){
		fprintf(stderr, "CASE 2\n");
		swapreg();
		gencode_helper(tree->right, tree);
		regstack_t *R = popreg();
		gencode_helper(tree->left, tree);
		genasm(outfile, tree, R->reg, regstack->reg);
		pushreg(R->reg);
		swapreg();
	}
	else if(tree->left->rval >= tree->right->rval){
		fprintf(stderr, "CASE 3\n");
		gencode_helper(tree->left, tree);
		regstack_t *R = popreg();
		gencode_helper(tree->right, tree);
		genasm(outfile, tree, regstack->reg, R->reg);
		pushreg(R->reg);
	}
	else if((tree->left->rval > MAXREG) && (tree->right->rval > MAXREG)){
		fprintf(stderr, "CASE 4\n");
	}
	else fprintf(stderr, "NUTHIN!\n");

}

// For now print of the rules triggered
void gencode(tree_t *tree){
	regstack = NULL;
	if(tree->rval >= 1) pushreg(R1);
	if(tree->rval = 2) pushreg(R0);

	gencode_helper(tree, tree);
	regflush(regstack);
}
