%{
 
/*
 * Parser.y file
 * To generate the parser run: "bison Parser.y"
 */
 
#include "epsi.h"

int linenum=1;

int yyerror() {
    // Add error handling routine as needed
	printf("Syntax error\n");
}
 
%}


%output  "parser.c"
%defines "parser.tab.h"
 


%union 
{
    int value;
    char *identifier;
}
 

 
%token <identifier> IDENTIFIER 
%token NUM_CONST LBRACKET RBRACKET COLON POINT SC
%token INT CHAR FLOAT FINPUT FOUTPUT VOID

%type <value> datum_type
 
%%
 
program: 
	  datum_list ;

datum_list: 
	  datum
	| datum_list datum ;

datum : 
	  datum_type IDENTIFIER LBRACKET datum_body RBRACKET	{insert_datum_ir($1,$2);};

datum_type:
	  		{$$=TYPE_DATUM;}
	| FINPUT 	{$$=TYPE_FINPUT;}
	| FOUTPUT 	{$$=TYPE_FOUTPUT;} ;

datum_body: 
	  statement_list ;

statement_list: 
	  statement
	| statement_list statement ;

statement: 
	  type IDENTIFIER COLON assignment SC 
	| type IDENTIFIER SC 
	| VOID COLON assignment SC;

type: 
	  INT | CHAR | FLOAT ;

assignment: 
	  NUM_CONST 
	| VOID 
	| IDENTIFIER ;






%%


















