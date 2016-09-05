%{
 
/*
 * Parser.y file
 * To generate the parser run: "bison Parser.y"
 */
 
#include "epsi.h"

int linenum=1;

int yyerror() {
    // Add error handling routine as needed
	printf("Syntax error at line %d.\n",linenum);
}
 
%}


%output  "parser.c"
%defines "parser.tab.h"
 


%union 
{
	int value;	
	char *identifier;
	condition *cond;
	conditions_list *cond_list;
}
 

 
%token <identifier> IDENTIFIER 
%token NUM_CONST LBRACKET RBRACKET COLON POINT SC
%token INT CHAR FLOAT FINPUT FOUTPUT VOID TERMINAL

%type <value> datum_type assignment type
%type <cond> statement
%type <cond_list> statement_list; 
 
%%
 
program: 
	  datum_list ;

datum_list: 
	  datum
	| datum_list datum ;

datum : 
	  datum_type IDENTIFIER LBRACKET statement_list RBRACKET	{insert_datum_ir($1,$2,$4);};

datum_type:
	  		{$$=TYPE_DATUM;}
	| FINPUT 	{$$=TYPE_FINPUT;}
	| FOUTPUT 	{$$=TYPE_FOUTPUT;} 
	| TERMINAL	{$$=TYPE_TERMINAL;} ;

statement_list: 
	  statement			{$$=new_condition_list($1);}
	| statement_list statement	{$$=append_condition($1,$2);};

statement: 
	  type IDENTIFIER COLON assignment SC 	{$$=new_condition($4,$1);}
	| type IDENTIFIER SC 			{$$=new_condition(CONDITION_INPUT,$1);}
	| VOID COLON assignment SC		{$$=new_condition($3,ELEMENT_VOID);};

type: 
	  INT	{$$=ELEMENT_INTEGER;}
	| CHAR 	{$$=ELEMENT_CHAR;}
	| FLOAT {$$=ELEMENT_FLOAT;};

assignment: 
	  NUM_CONST 	{$$=CONDITION_NUM_CONST;}
	| VOID 		{$$=CONDITION_VOID;}
	| IDENTIFIER 	{$$=CONDITION_IDENTIFIER;};






%%


















