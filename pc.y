%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "node.h"
#include "scope.h"
#include "tree.h"
#include "y.tab.h"

extern scope_t *top_scope;
extern node_t *tmp;
extern namelist_t *nametmp;

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
%token	IF THEN ELSE
%token	WHILE DO
%token  FOR TO

%token	FUNCTION_CALL
%token	ARRAY_ACCESS
%token	COMMA


%type <tval> expression_list
%type <tval> expression
%type <tval> simple_expression
%type <tval> term
%type <tval> factor

%type <tval> array_variable


%%

program:
	{ top_scope = scope_push(top_scope); }
	PROGRAM ID '(' identifier_list ')' ';'

	declarations
	subprogram_declarations
	compound_statement
	'.'
	{ top_scope = scope_pop(top_scope); }
	;

identifier_list
	: ID
		{ scope_insert(top_scope, $1); }
	| identifier_list ',' ID
		{ scope_insert(top_scope, $3); }
	;

declarations
	: declarations VAR identifier_list ':' type ';'
	| /* empty */
	;

type
	: standard_type
	| ARRAY '[' INUM DOTDOT INUM ']' OF standard_type
	;

standard_type
	: INTEGER
	| REAL
	;

subprogram_declarations
	: subprogram_declarations subprogram_declaration ';'
	| /* empty */
	;

subprogram_declaration
	: subprogram_head declarations subprogram_declarations compound_statement
		{ top_scope = scope_pop(top_scope); }
	;

subprogram_head
	: FUNCTION ID 
		{ top_scope = scope_push(top_scope); }
		arguments ':' standard_type ';'
	| PROCEDURE ID 
		{ top_scope = scope_push(top_scope); }
		arguments ';'
	;

arguments
	: '(' parameter_list ')'
	;

parameter_list
	: identifier_list ':' type
	| parameter_list ';' identifier_list ':' type
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
		/*{ fprintf(stderr, "\n\nPRINTING TREE:\n");
			print_tree($3,0); 
		  fprintf(stderr, "\n\n");
		}*/
	| procedure_statement
	| compound_statement
	| { top_scope = scope_push(top_scope); }
	  IF expression THEN statement ELSE statement
	  { top_scope = scope_pop(top_scope); }
	| { top_scope = scope_push(top_scope); }
	  WHILE expression DO statement 
	  { top_scope = scope_pop(top_scope); }
	| { top_scope = scope_push(top_scope); }
	  FOR variable ASSIGNOP simple_expression TO simple_expression DO statement 
	  { top_scope = scope_pop(top_scope); }

	;

variable
	: ID
	/*| ID '[' expression ']' {fprintf(stderr, "\nTriggering the gay one.\n");}*/
	| array_variable
	;

/*this is duplicate of the array access in factor, but necessary to 
access left side of assignop statement.*/
array_variable
	: ID '[' expression ']'
		{ 
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
	| ID '(' expression_list ')'
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
		{ $$ = make_op(ADDOP, $2, $1, $3); }
	;

term
	: factor
		{ $$ = $1; }
	| term MULOP factor
		{ $$ = make_op(MULOP, $2, $1, $3); }
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
	| ID '(' expression_list ')'
		{ 
			if ((tmp = scope_search_all(top_scope, $1)) == NULL) {
				fprintf(stderr, "Name %s used but not defined\n", $1);
				exit(1);
			}
			$$ = make_tree(FUNCTION_CALL, make_id(tmp), $3); 
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

main()
{
	top_scope = NULL;
	tmp = NULL;

	yyparse();
	
}

yyerror(char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(1);
}

