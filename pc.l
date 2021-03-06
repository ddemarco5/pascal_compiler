%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "node.h"
#include "scope.h"
#include "tree.h"
#include "y.tab.h"

void echo(char *);

%}

ident		[a-zA-Z][a-zA-Z0-9]*
number		[0-9]+
whitespace	[ \t]+
%x COMMENT

%%

"program"	{ echo("PROGRAM"); return PROGRAM; }
"var"		{ echo("VAR"); return VAR; }
"array"		{ echo("ARRAY"); return ARRAY; }
"of"		{ echo("OF"); return OF; }
"integer"	{ echo("INTEGER"); return INTEGER; }
"real"		{ echo("REAL"); return REAL; }
"function"	{ echo("FUNCTION"); return FUNCTION; }
"procedure"	{ echo("PROCEDURE"); return PROCEDURE; }

"read"		{ echo("READ"); return READ; }
"write"		{ echo("WRITE"); return WRITE; }

"begin"		{ echo("BBEGIN"); return BBEGIN; }
"end"		{ echo("END"); return END; }
"if"		{ echo("IF"); return IF; }
"then"		{ echo("THEN"); return THEN; }
"else"		{ echo("ELSE"); return ELSE; }
"while"		{ echo("WHILE"); return WHILE; }
"do"		{ echo("DO"); return DO; }
"not"		{ echo("NOT"); return NOT; }
"for"		{ echo("FOR"); return FOR; }
"to"		{ echo("TO"); return TO; }
".."		{ echo("DOTDOT"); return DOTDOT; }
":="		{ echo("ASSIGNOP"); return ASSIGNOP; }

"="			{ fprintf(stderr, "RELOP[%s]", yytext); return RELOP; }
"<>"		{ fprintf(stderr, "RELOP[%s]", yytext); return RELOP; }
"<"			{ fprintf(stderr, "RELOP[%s]", yytext); return RELOP; }
"<="		{ fprintf(stderr, "RELOP[%s]", yytext); return RELOP; }
">"			{ fprintf(stderr, "RELOP[%s]", yytext); return RELOP; }
">="		{ fprintf(stderr, "RELOP[%s]", yytext); return RELOP; }

"or"		{ fprintf(stderr, "ADDOP[%s]", yytext); yylval.opval = OR; return ADDOP; }
"+"			{ fprintf(stderr, "ADDOP[%s]", yytext); yylval.opval = PLUS; return ADDOP; }
"-"			{ fprintf(stderr, "ADDOP[%s]", yytext); yylval.opval = MINUS; return ADDOP; }

"and"		{ fprintf(stderr, "MULOP[%s]", yytext); yylval.opval = AND; return MULOP; }
"*"			{ fprintf(stderr, "MULOP[%s]", yytext); yylval.opval = STAR; return MULOP; }
"/"			{ fprintf(stderr, "MULOP[%s]", yytext); yylval.opval = SLASH; return MULOP; }

{number}	{ fprintf(stderr, "INUM[%d]", yylval.ival = atoi(yytext)); return INUM; }
{number}"."{number}	{ fprintf(stderr, "RNUM[%f]", yylval.rval = atof(yytext)); return RNUM; }
{ident}		{ fprintf(stderr, "ID[%s]", yylval.sval = strdup(yytext)); return ID; }

{whitespace}	{ ; }
\n			{ fprintf(stderr,"%s", yytext); }
.			{ fprintf(stderr, "{%c}", yytext[0]); return yytext[0]; }

"//"[^\n]*\n    { /* do nothing for comments*/; }
"(*"            { BEGIN(COMMENT); }
<COMMENT>\n     { ; }
<COMMENT>.      { ; }
<COMMENT>"*)"   { BEGIN(INITIAL); } /* reset lex to normal state */

%%

void echo(char *message)
{
	fprintf(stderr, "{%s}", message);
}


