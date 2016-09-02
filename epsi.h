#ifndef EPSI_h
#define EPSI_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//scanner
void yyset_in (FILE *  in_str );
int yylex (void);

//parser
int yyparse();

//ir 
#define TYPE_DATUM 	0
#define TYPE_FINPUT 	1
#define TYPE_FOUTPUT 	2
#define TYPE_TERMINAL	3


struct condition
{
};
typedef struct condition condition;

struct conditions_list
{
	condition datum_condition; 
	struct conditions_list *next;
};
typedef struct conditions_list conditions_list;

//structure that represents a datum
struct datum_ir
{
	int type; //datum, finput, foutput
	char *identifier; //datum name
	conditions_list *conditions_head;
};
typedef struct datum_ir datum_ir;

//List of program data
struct datum_list
{
	datum_ir datum;
	struct datum_list *next;
};
typedef struct datum_list datum_list;

void insert_datum_ir(int type,char *name);



//runtime
void execute();


//debug
void dbg_print_ir();



#endif
