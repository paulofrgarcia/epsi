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
	struct assignment_data assign_data;
}
 

 
%token <identifier> IDENTIFIER 
%token <value> NUM_CONST
%token LBRACKET RBRACKET COLON POINT SC
%token INT CHAR FLOAT FINPUT FOUTPUT VOID TERMINAL

%type <value> datum_type type
%type <cond> statement
%type <cond_list> statement_list; 
%type <assign_data> assignment
 
%%
 
program: 
	  datum_list ;

datum_list: 
	  datum
	| datum_list datum ;

datum : 
	datum_type IDENTIFIER COLON statement_list 	{insert_datum_ir($1,$2,$4);
	  /*datum_type IDENTIFIER LBRACKET statement_list RBRACKET	{insert_datum_ir($1,$2,$4);};*/};

datum_type:
	  		{$$=TYPE_DATUM;}
	| FINPUT 	{$$=TYPE_FINPUT;}
	| FOUTPUT 	{$$=TYPE_FOUTPUT;} 
	| TERMINAL	{$$=TYPE_TERMINAL;} ;

statement_list: 
	  statement			{$$=new_condition_list($1);}
	| statement_list statement	{$$=append_condition($1,$2);};

statement: 
	  type IDENTIFIER COLON assignment SC 	{$$=new_condition($4.value,$1,$4.identifier,$4.num_const);}
	| type IDENTIFIER SC 			{$$=new_condition(CONDITION_INPUT,$1,NULL,0);}
	| VOID COLON assignment SC		{$$=new_condition($3.value,ELEMENT_VOID,$3.identifier,$3.num_const);};

type: 
	  INT	{$$=ELEMENT_INTEGER;}
	| CHAR 	{$$=ELEMENT_CHAR;}
	| FLOAT {$$=ELEMENT_FLOAT;};

assignment: 
	  NUM_CONST 	{$$.value=CONDITION_NUM_CONST; $$.num_const=$1;}
	| VOID 		{$$.value=CONDITION_VOID;}
	| IDENTIFIER 	{$$.value=CONDITION_IDENTIFIER; $$.identifier=$1;};






%%


















