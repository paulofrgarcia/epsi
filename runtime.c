#include"epsi.h"

extern int errors;

//Checks if condition is true
int condition_true(condition *c)
{
	switch(c->type)
	{
	case(CONDITION_VOID):
	{
		return 1;
	}	
	default: return 0;
	}
	return 0;
}

//Checks if datum is alive
int is_alive(datum_ir *d)
{
	int alive=1;
	conditions_list *cond;

	for(cond=d->conditions_head;cond!=NULL;cond=cond->next)
	{
		if(!condition_true(&(cond->datum_condition)))
			return 0;
	}

	return 1;
}


extern datum_list *datum_list_head;
//Selects next live datum
datum_ir *select_datum()
{
	datum_list *iter;

	for(iter=datum_list_head;iter!=NULL;iter=iter->next)
	{
		if(is_alive(&(iter->datum)))
			return &(iter->datum);
	}

	//Could not find a live datum
	errors++;
	return NULL;
}

//Creates datum instance
void run_datum(datum_ir *d)
{
}





//Selects next datum to execute and runs it
//returns 0 if executing terminal datum
int execute_one()
{
	datum_ir *i=select_datum();

	if(errors!=0)
	{
		//no live data: blocked
		printf("Error: program deadlocked.\n");
		return 0;
	}

	if(i->type==TYPE_TERMINAL)
		return 0;
	else
	{
		run_datum(i);
		return 1;
	}
}

void execute()
{
	while(execute_one()!=0);
	return;
}
