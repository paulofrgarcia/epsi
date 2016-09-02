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

//structure that represents a datum
struct datum_ir
{
	int type; //datum, finput, foutput
	char *identifier; //datum name
};
typedef struct datum_ir datum_ir;

void insert_datum_ir(int type,char *name);



//runtime
void execute();


//debug
void dbg_print_ir();



#endif
