#include"epsi.h"

extern int errors;

//checks if term is true
int term_true(struct term_data *term)
{
	switch(term->type)
	{
	case(TERM_MEMBER):
	{
		if(get_datum((char *)(term->data1))==NULL)
		{
			printf("Error: request for non-existing datum.\n");
			errors++;
			return 0;
		}
		if((get_datum((char *)(term->data1))->state)==STATE_ALIVE)
			return 1;
		else
			return 0;
	} break;
	case(TERM_NUM_CONST):
	{
		return 1;
	} break;
	case(TERM_TIMES):
	case(TERM_DIVISION):
	case(TERM_MINUS):
	case(TERM_PLUS):
	{
		if(!term_true((struct term_data *)(term->data1)))
			return 0;
		if(!term_true((struct term_data *)(term->data2)))
			return 0;
		return 1;
	} break;
	default:
		return 0;
	}
	return 0;
}

//Checks if condition is true
int condition_true(condition *c)
{
	switch(c->type)
	{
	case(CONDITION_VOID):
	{
		return 1;
	} break;	
	case(CONDITION_INPUT):
	{
		return 1;
	} break;
	case(CONDITION_NUM_CONST):
	{
		return 1;
	} break;
	case(CONDITION_IDENTIFIER):
	{
		//true if "identifier" datum is alive
		if(get_datum(c->datum_dependency)==NULL)
		{
			printf("Error: request for non-existing datum \"%s\".\n",c->datum_dependency);
			errors++;
			return 0;
		}
		if(((get_datum(c->datum_dependency))->state)==STATE_ALIVE)
			return 1;
		else
			return 0;
	} break;
	case(CONDITION_TERM):
	{
		return term_true(c->term);



		//true if "identifier" datum is alive
		
			
	} break;
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
		scanf("%d",(int *)(c->elem_data));
		return;
	}
	default: return;
	}
}

void condition_num_const(condition *c, datum_ir *d)
{
	//for datum, finput and terminal, do nothing
	//data was created at compile time
	
	//for output type, output data
	if(d->type!=TYPE_FOUTPUT)
		return;
	
	switch(c->elem_type)
	{
	case(ELEMENT_INTEGER):
	{
		printf("%d\n",*(int *)(c->elem_data));
		return;
	} break;
	default: return;
	}
}

void *calculate_term(struct term_data *term)
{
	void *result=NULL;
	int tmp1, tmp2;
	
	switch(term->type)
	{
	case(TERM_MEMBER):
	{
		//get data from datum
		result=get_datum_elem_data((char *)(term->data1), (char *)(term->data2));
	} break;
	case(TERM_NUM_CONST):
	{
		result=(void *)(int *)malloc(sizeof(int));
		*(int *)result=*(int *)(term->data1);
	} break;
	case(TERM_PLUS):
	{
		result=(void *)(int *)malloc(sizeof(int));
		
		tmp1=*(int *)(calculate_term((struct term_data*)(term->data1)));
		tmp2=*(int *)(calculate_term((struct term_data*)(term->data2)));
		*(int *)result= tmp1 + tmp2;
	} break;
	case(TERM_MINUS):
	{
		result=(void *)(int *)malloc(sizeof(int));
		
		tmp1=*(int *)(calculate_term((struct term_data*)(term->data1)));
		tmp2=*(int *)(calculate_term((struct term_data*)(term->data2)));
		*(int *)result= tmp1 - tmp2;
	} break;
	case(TERM_TIMES):
	{
		result=(void *)(int *)malloc(sizeof(int));
		
		tmp1=*(int *)(calculate_term((struct term_data*)(term->data1)));
		tmp2=*(int *)(calculate_term((struct term_data*)(term->data2)));
		*(int *)result= tmp1 * tmp2;
	} break;
	case(TERM_DIVISION):
	{
		result=(void *)(int *)malloc(sizeof(int));
		
		tmp1=*(int *)(calculate_term((struct term_data*)(term->data1)));
		tmp2=*(int *)(calculate_term((struct term_data*)(term->data2)));
		*(int *)result= tmp1 / tmp2;
	} break;
	default:
	{
		printf("result");
		return;
	}
	}

	return result;
}

void process_term(condition *c, datum_ir *d)
{
	//calculate term
	c->elem_data=calculate_term(c->term);

	//if output datum, output term
	if(d->type==TYPE_FOUTPUT)
	{
		switch(c->elem_type)
		{
		case(ELEMENT_INTEGER):
		{
			printf("%d\n",*(int *)(c->elem_data));
			return;
		} break;
		default: return;
		}
	}

	return;
}

void execute_statement(condition *c, datum_ir *d)
{
	
	switch(c->type) //what kind of operation
	{
	case(CONDITION_INPUT):
	{
		condition_input(c);
		return;
	} break;
	case(CONDITION_TERM):
	{
		process_term(c,d);
		return;
	} break;
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
		execute_statement(&(c->datum_condition),d);
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
