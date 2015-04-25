%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "node.h"
#include "scope.h"
#include "tree.h"
#include "arglist.h"
#include "y.tab.h"

extern scope_t *top_scope;
extern node_t *tmp;
extern namelist_t *nametmp;
extern funcstack_t *funcstacktmp;

%}

%union {
	/* scanner */
	int ival;
	float rval;
	char *sval;
	int opval;

/* semantic + gencode */
	tree_t *tval;
}


%token <ival>	INUM
%token <rval>	RNUM
%token <sval>	ID

%token	ASSIGNOP
%token <opval>	RELOP
%token <opval>	ADDOP
%token <opval>	MULOP
%token	NOT

%token	LT LE GT GE EQ NE
%token	OR PLUS MINUS
%token	AND STAR SLASH

%token	PROGRAM
%token	VAR
%token	ARRAY OF DOTDOT
%token	INTEGER REAL
%token	FUNCTION PROCEDURE
%token	BBEGIN END
%nonassoc NO_ELSE
%nonassoc ELSE
%token	IF THEN

%token	WHILE DO
%token  	FOR TO

%token	FUNCTION_CALL
%token	ARRAY_ACCESS
%token	COMMA


%type <tval> expression_list
%type <tval> expression
%type <tval> simple_expression
%type <tval> term
%type <tval> factor
%type <tval> array_variable
%type <tval> variable
/*We set standard_type as an ival to transfer INTEGER and REAL information*/
%type <ival> standard_type
/*This is set as a string so we can return the ID in subprogram_head*/
%type <sval> subprogram_head

%%

program:
	{
	 top_scope = scope_push(top_scope);
	 nametmp = create_namelist();
	 funcstacktmp = init_funcstack();
	}
	/* Right now we aren't putting a type for input and output in the symbol table */
	PROGRAM ID '(' identifier_list ')' ';' 
	{ print_names(nametmp); flush_namelist(nametmp); }
	declarations
	subprogram_declarations
	compound_statement
	'.'
	{ top_scope = scope_pop(top_scope); }
	;

identifier_list
	: ID
		{ 
		scope_insert(top_scope, $1);
		insert_name(nametmp, $1);
		}
	| identifier_list ',' ID
		{ 
		scope_insert(top_scope, $3); 
		insert_name(nametmp, $3);
		}
	;

declarations
	: 
	declarations VAR {nametmp = create_namelist();} identifier_list ':' type ';'
	{print_names(nametmp); flush_namelist(nametmp);}
	| /* empty */
	;

type
	: standard_type
	{
		if(tmp != NULL){
		if((tmp->type == FUNCTION) || (tmp->type == PROCEDURE))
			add_arg(tmp, $1);
     			fprintf(stderr, "HALLOOOO1: %d\n", tmp->args[0]);
		}
	}
	| ARRAY '[' INUM DOTDOT INUM ']' OF standard_type
    	{
		if(tmp != NULL){
	     	if((tmp->type == FUNCTION) || (tmp->type == PROCEDURE))
	     		add_arg(tmp, $8);
     			fprintf(stderr, "HALLOOOO2: %d\n", tmp->args[0]);
		}
	     	tmp = scope_search_all(top_scope, nametmp->name);
	     	tmp->astart = $3;
	     	tmp->aend = $5;
	     	tmp->rtype = $8;
    	}
	;

standard_type
	: INTEGER 
	{
		$$ = INTEGER;
		typify_namelist(top_scope, nametmp, INTEGER);
	}
	| REAL 
	{
		$$ = REAL;
		typify_namelist(top_scope, nametmp, REAL);
	}
	;

subprogram_declarations
	: subprogram_declarations subprogram_declaration ';'
	| /* empty */
	;

subprogram_declaration
	: subprogram_head declarations subprogram_declarations compound_statement
		{ 
            top_scope = scope_pop(top_scope);
            tmp = scope_search(top_scope, $1);
            //fprintf(stderr, "ASDASDASD: %s.\n", tmp->name);
            if((tmp->type == FUNCTION) && (tmp->returned != 1)){
                fprintf(stderr, "Function is never returned.\n");
                exit(1);
            }
        }
	;

subprogram_head
	: FUNCTION ID 
		{	
			scope_insert(top_scope, $2);
			nametmp = create_namelist();
			insert_name(nametmp, $2);
			typify_namelist(top_scope, nametmp, FUNCTION);
			print_names(nametmp);
			flush_namelist(nametmp);
			// This will get our node for argument adding down in param list
			tmp = scope_search(top_scope, $2);

			top_scope = scope_push(top_scope);
		}
		arguments ':' standard_type ';'
		{
            		$$ = $2;
			if(($6 != INTEGER) && ($6 != REAL)){
				fprintf(stderr, "Function return type not INTEGER or REAL.");
				exit(1);
			}
			tmp = scope_search_all(top_scope, $2);
			tmp->rtype = $6;
		}
	| PROCEDURE ID 
		{ 
			scope_insert(top_scope, $2);
			nametmp = create_namelist();
			insert_name(nametmp, $2);
			typify_namelist(top_scope, nametmp, PROCEDURE);
			print_names(nametmp);
			flush_namelist(nametmp);
			// This will get our node for argument adding down in param list
			tmp = scope_search(top_scope, $2);

			top_scope = scope_push(top_scope);
		}
		arguments ';' 
        { 
            $$ = $2;
        }
	| PROCEDURE ID ';'
		{ 
            		$$ = $2;
			scope_insert(top_scope, $2);
			nametmp = create_namelist();
			insert_name(nametmp, $2);
			typify_namelist(top_scope, nametmp, PROCEDURE);
			print_names(nametmp);
			flush_namelist(nametmp);
			top_scope = scope_push(top_scope);
		}
	;

arguments
	: '(' parameter_list ')'
	;

parameter_list
	: {nametmp = create_namelist();}
	identifier_list ':' type
	{
		print_names(nametmp); flush_namelist(nametmp);
	}
	| parameter_list ';'{nametmp = create_namelist();} identifier_list ':' type
	{
		print_names(nametmp); flush_namelist(nametmp);
	}
	;


/* statement */

compound_statement
	: BBEGIN optional_statements END 
	;

optional_statements
	: statement_list
	| /* empty */
	;

statement_list
	: statement
	| statement_list ';' statement
	;

statement
	: variable ASSIGNOP expression
		{ 	/*
			fprintf(stderr, "\n\nPRINTING TREE\n");
			print_tree($3,0);
			fprintf(stderr, "\n\n");*/
            // Begin left/right side type checking
            int x,y;  //x will be the left side check, y will be the right.
            x = y = -1;
            if(($3->type == FUNCTION) || ($3->type == ARRAY_ACCESS)){
                y = $3->left->attribute.sval->rtype;
            }
            else{
                    y = get_branch_type($3);
                    if(y == RNUM) y = REAL;
                    if(y == INUM) y = INTEGER;
                }

        
            //fprintf(stderr, "HAY BETCH: %d.\n", get_branch_type($3));
            
            //check if left side if function and use rtype
                //fprintf(stderr, "TEEEEEEEEEEEEEST!\n");
            if($1->type == ARRAY_ACCESS){
                x = $1->left->attribute.sval->rtype;
            }
            if($1->attribute.sval != NULL){
                if($1->attribute.sval->type == FUNCTION){
                    x = $1->attribute.sval->rtype;
                }
                else if($1->type == ARRAY_ACCESS){
                    x = $1->attribute.sval->rtype;
                }

                else{
                    x = $1->attribute.sval->type;
                }
            }
            // Our main checks 
            fprintf(stderr, "CHECKING %d AGAINST %d\n", x, y);
            if(y == PROCEDURE){
                fprintf(stderr, "Procedures do not return values.\n");
            }
            if(x != y){
                fprintf(stderr, "Invalid type assignment attempted (%d, %d).\n", x, y);
                exit(1);
            }
            // End left/right side type checking

            if($1->attribute.sval != NULL){
                if($1->attribute.sval->type == FUNCTION)
                    $1->attribute.sval->returned = 1;
            }
           

            if(($1->attribute.sval != NULL) && ($1->attribute.sval->type == PROCEDURE)){
                tmp = scope_search(top_scope, $1->attribute.sval->name);
                if(tmp == NULL){
                    fprintf(stderr,"Procedure attempted to return.\n");
                    exit(1);
                }

            }

            if(($1->attribute.sval != NULL) && ($1->attribute.sval->type != FUNCTION)){
                tmp = scope_search(top_scope, $1->attribute.sval->name);
                if(tmp == NULL){
                    fprintf(stderr,"[F] Attempt to modify variable out of scope.\n");
                    exit(1);
                }
            }
            if($3->type == PROCEDURE){
                fprintf(stderr, "Procedures cannot return values.\n");
                exit(1);
            }

        }
	| procedure_statement
	| compound_statement
	| IF expression THEN statement %prec NO_ELSE
		{ 
			if($2->type != RELOP){
				fprintf(stderr, "ERROR: Non-boolean in expression.\n");
				exit(1);
			}
		}
	| IF expression THEN statement ELSE statement
		{ 
			if($2->type != RELOP){
				fprintf(stderr, "ERROR: Non-boolean in expression.\n");
				exit(1);
			}
		}
	| WHILE expression DO statement 
	  { 
			if($2->type != RELOP){
				fprintf(stderr, "ERROR: Non-boolean in expression.\n");
				exit(1);
			}
	  }
	| FOR variable ASSIGNOP simple_expression TO simple_expression DO statement 
	  { 
			fprintf(stderr, "LKDLKJSDLKJD: %d,%d\n", $2->attribute.sval->type, $4->type);
			if($4->type != $6->type){
				fprintf(stderr, "ERROR: Expressions not same type.\n");
				exit(1);
			}
			if(($2->attribute.sval->type != REAL && $4->type == RNUM)||($2->attribute.sval->type != INTEGER && $4->type == INUM)){
				fprintf(stderr, "ERROR: Incompatable variable in FOR statement.\n");
				exit(1);
			}
	  }
	;

variable
	: ID
	{	
		if ((tmp = scope_search_all(top_scope, $1)) == NULL) {
			fprintf(stderr, "Assignment name %s used but not defined\n", $1);
			exit(1);
		}
		$$ = make_id(tmp);

	}
	/*| ID '[' expression ']' {fprintf(stderr, "\nTriggering the gay one.\n");}*/
	| array_variable
	;

/*this is duplicate of the array access in factor, but necessary to 
access left side of assignop statement.*/
array_variable
	: ID '[' expression ']'
		{
			int x;
			switch($3->type) {
				case ID:
					x = ($3->attribute.sval)->type;
					if( x != INTEGER){
						fprintf(stderr, "Non integer type used to index array.\n");
						exit(1);
					}
					break;
				case RNUM:
					fprintf(stderr, "Non integer type used to index array.\n");
					exit(1);
					break;
			}

			if ((tmp = scope_search_all(top_scope, $1)) == NULL) {
				fprintf(stderr, "Name %s used but not defined\n", $1);
				exit(1);
			}

			fprintf(stderr, "\n\nPRINTING LEFT SIDE ARRAY_ACCESS TREE:\n");
			print_tree($3,0); 
			fprintf(stderr, "\n\n");

			$$ = make_tree(ARRAY_ACCESS, make_id(tmp), $3); 
		}
	;

procedure_statement
	: ID
	| ID 
	{
		funcstacktmp = push_funcstack(funcstacktmp, scope_search_all(top_scope, $1));
		print_funcstack(funcstacktmp);
	} '(' expression_list ')'
	{
		funcstacktmp = pop_funcstack(funcstacktmp);
		print_funcstack(funcstacktmp);
	}
	;


/* expression */

expression_list
	: expression
		{ $$ = $1;}
	| expression_list ',' expression
		{ $$ = make_tree(COMMA, $1, $3);}
	;

expression
	: simple_expression
		{ $$ = $1; }
	| simple_expression RELOP simple_expression
		{ $$ = make_op(RELOP, $2, $1, $3); }
	;

simple_expression
	: term
		{ $$ = $1; }
	| simple_expression ADDOP term
		{ 
            $$ = make_op(ADDOP, $2, $1, $3); 
        }
	;

term
	: factor
		{ $$ = $1; }
	| term MULOP factor
		{ 
            $$ = make_op(MULOP, $2, $1, $3); 
        }
	;

factor
	: ID
		{
			if ((tmp = scope_search_all(top_scope, $1)) == NULL) {
				fprintf(stderr, "Name %s used but not defined\n", $1);
				exit(1);
			}
			fprintf(stderr, "SCOPE TABLE TYPE: %d\n", tmp->type);
			$$ = make_id(tmp); 
		}
	| ID '[' expression ']'
		{ 
			if ((tmp = scope_search_all(top_scope, $1)) == NULL) {
				fprintf(stderr, "Name %s used but not defined\n", $1);
				exit(1);
			}

	fprintf(stderr, "\n\nPRINTING ARRAY_ACCESS TREE:\n");
			print_tree($3,0); 
			fprintf(stderr, "\n\n");

	$$ = make_tree(ARRAY_ACCESS, make_id(tmp), $3); 
		}
	| ID 
	{
		funcstacktmp = push_funcstack(funcstacktmp, scope_search_all(top_scope, $1));
		print_funcstack(funcstacktmp);
	} 
	'(' expression_list ')'
		{ 
			if ((tmp = scope_search_all(top_scope, $1)) == NULL) {
				fprintf(stderr, "Name %s used but not defined\n", $1);
				exit(1);
			}
			$$ = make_tree(tmp->type, make_id(tmp), $4); 
	    		
			funcstacktmp = pop_funcstack(funcstacktmp);
			print_funcstack(funcstacktmp);
		}
	| INUM
		{ $$ = make_inum($1); }
	| RNUM
		{ $$ = make_rnum($1); }
	| '(' expression ')'
		{ $$ = $2; }
	| NOT factor
		{ $$ = make_tree(NOT,NULL,NULL); }
	;

%%

scope_t *top_scope;
node_t *tmp;
namelist_t *nametmp;
funcstack_t *funcstacktmp;

main()
{
	top_scope = NULL;
	tmp = NULL;
	nametmp = NULL;
	funcstacktmp = NULL;

yyparse();

}

yyerror(char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}

