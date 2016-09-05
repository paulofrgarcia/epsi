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
	case(CONDITION_INPUT):
	{
		return 1;
	}
	default: return 0;
	}
	return 0;
}

//Checks if datum is schedulable
int is_schedulable(datum_ir *d)
{
	conditions_list *cond;

	if(d->state!=STATE_UNBORN)
		//already been created
		return 0;

	for(cond=d->conditions_head;cond!=NULL;cond=cond->next)
	{
		if(!condition_true(&(cond->datum_condition)))
			return 0;
	}

	return 1;
}


extern datum_list *datum_list_head;
//Selects next schedulable datum
datum_ir *select_datum()
{
	datum_list *iter;

	for(iter=datum_list_head;iter!=NULL;iter=iter->next)
	{
		if(is_schedulable(&(iter->datum)))
			return &(iter->datum);
	}

	//Could not find a live datum
	errors++;
	return NULL;
}

void condition_input(condition *c)
{
	switch(c->elem_type)
	{
	case(ELEMENT_INTEGER):
	{
		c->elem_data=(int *)malloc(sizeof(int));
		scanf("%d",(int *)(c->elem_data));
		return;
	}
	default: return;
	}
}

void execute_statement(condition *c)
{
	switch(c->type) //what kind of operation
	{
	case(CONDITION_INPUT):
	{
		condition_input(c);
		return;
	}
	default: return;
	}
}

//Creates datum instance
void run_datum(datum_ir *d)
{
	conditions_list *c;
	
	printf("Creating datum %s...\n",d->identifier);

	for(c=d->conditions_head;c!=NULL;c=c->next)
	{
		execute_statement(&(c->datum_condition));
	}
	d->state=STATE_ALIVE;
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
	{
		printf("Terminal datum found.\n");
		return 0;
	}
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
