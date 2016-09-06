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

//static checks
void static_checks();

struct assignment_data
{
	int value;
	char *identifier;
	int num_const;
};

//ir

//datum states
#define STATE_UNBORN	0
#define STATE_ALIVE	1
#define STATE_DEAD	2

//datum types 
#define TYPE_DATUM 	0
#define TYPE_FINPUT 	1
#define TYPE_FOUTPUT 	2
#define TYPE_TERMINAL	3

//condition types
#define CONDITION_VOID		0
#define CONDITION_NUM_CONST	1
#define CONDITION_IDENTIFIER	2
#define CONDITION_INPUT		3

//elements types
#define	ELEMENT_VOID	0
#define	ELEMENT_CHAR	1
#define	ELEMENT_INTEGER	2
#define	ELEMENT_FLOAT	3


//datum condition
struct condition
{
	int type;//validity type, i.e., start condition
	int elem_type; //type of element (int, char, etc.)
	void *elem_data; //we'll want to cast this
	char *datum_dependency; //identifier of datum depended upon
};
typedef struct condition condition;

//List of datum conditions
struct conditions_list
{
	condition datum_condition; 
	struct conditions_list *next;
};
typedef struct conditions_list conditions_list;


condition *new_condition(int type, int elem_type, char *d_name, int num_const);

conditions_list *new_condition_list(condition *c);
conditions_list *append_condition(conditions_list *list,condition *c);


//structure that represents a datum
struct datum_ir
{
	int state;
	int type; //datum, finput, foutput, terminal
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

void insert_datum_ir(int type,char *name, conditions_list *list);

//returns pointer to datum named "s"
//NULL if it doesn't exist
datum_ir *get_datum(char *s);


//runtime
void execute();


//debug
void dbg_print_ir();



#endif
