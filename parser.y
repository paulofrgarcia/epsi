%{
 
/*
 * Parser.y file
 * To generate the parser run: "bison Parser.y"
 */
 
#include "epsi.h"

int linenum=1;

char void_string[]="void";

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
	struct term_data *term_data;
}
 

 
%token <identifier> IDENTIFIER 
%token <value> NUM_CONST
%token LBRACKET RBRACKET COLON POINT SC
%token INT CHAR FLOAT FINPUT FOUTPUT VOID TERMINAL
%token PLUS MINUS TIMES DIVISION LPAR RPAR

%type <value> datum_type type
%type <cond> statement
%type <cond_list> statement_list
%type <assign_data> assignment
%type <term_data> term subterm factor
 
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
	  type IDENTIFIER COLON assignment SC 	{$$=new_condition($2,$4.value,$1,$4.identifier,$4.num_const,$4.term_data);}
	| type IDENTIFIER SC 			{$$=new_condition($2,CONDITION_INPUT,$1,NULL,0,NULL);}
	| VOID COLON assignment SC		{$$=new_condition(void_string,$3.value,ELEMENT_VOID,$3.identifier,$3.num_const,$3.term_data);};

type: 
	  INT	{$$=ELEMENT_INTEGER;}
	| CHAR 	{$$=ELEMENT_CHAR;}
	| FLOAT {$$=ELEMENT_FLOAT;};

assignment: 
	  VOID 		{$$.value=CONDITION_VOID;}
	| IDENTIFIER 	{$$.value=CONDITION_IDENTIFIER; $$.identifier=$1;}
	| term 		{$$.value=CONDITION_TERM; $$.term_data=$1;};


term:
	  term PLUS subterm 	{$$=create_term(TERM_PLUS,(void *)$1,(void *)$3);}
	| term MINUS subterm 	{$$=create_term(TERM_MINUS,(void *)$1,(void *)$3);}
	| subterm 		{$$=$1;} ;

subterm: 
	  subterm TIMES factor 		{$$=create_term(TERM_TIMES,(void *)$1,(void *)$3);}
	| subterm DIVISION factor 	{$$=create_term(TERM_DIVISION,(void *)$1,(void *)$3);}
	| factor 			{$$=$1;} ;


factor: 
	  LPAR term RPAR 		{$$=$2;}
	| IDENTIFIER POINT IDENTIFIER	{$$=create_term(TERM_MEMBER,$1,$3);}
	| NUM_CONST 			{$$=create_term(TERM_NUM_CONST,(void *)(&$1),NULL);} ;





%%


















