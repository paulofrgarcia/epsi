#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"epsi.h"

int errors=0;

int main(int argc, char *argv[])
{
	FILE *in_file;

	if(argc!=2)
	{	
		printf("Usage: epsi <source>\n");
		return 0;
	}
	if((in_file=fopen(argv[1],"r"))==NULL)
	{
		printf("Could not open source file \"%s\"\n",argv[1]);
		return 0;
	}
	printf("Parsing source \"%s\"...\n",argv[1]);

	yyset_in(in_file);

	//parse and generate ir here
	yyparse();

	if(errors!=0)
		return 0;
	
	//Perform static checks
	static_checks();

	if(errors!=0)
		return 0;


	dbg_print_ir();

	//generate executable

	printf("Running...\n");

	//execute here
	execute();

	return 0;
}
