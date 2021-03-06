#include"epsi.h"

extern int errors;

condition *new_condition(char *name, int type, int elem_type, char *d_name, int num_const, struct term_data *term)
{
	condition *cond=(condition *)malloc(sizeof(condition));
	cond->type=type;
	cond->elem_type=elem_type;
	cond->datum_dependency=d_name;
	cond->term=term;
	cond->elem_name=name;

	switch(cond->elem_type)
	{
	case(ELEMENT_INTEGER):
	{
		cond->elem_data=(int *)malloc(sizeof(int));
		*(int *)(cond->elem_data)=num_const;
	} break;
	default: return cond;
	}



	return cond;
}

//true if datum d has an element named s
int datum_has_element(datum_ir *d, char *s)
{
	conditions_list *list;
	if(d==NULL)
		return 0;
	if(s==NULL)
		return 0;

	for(list=d->conditions_head;list!=NULL;list=list->next)
	{
		if(strcmp(list->datum_condition.elem_name,s)==0)
			return 1;
	}

	return 0;
}

//returns pointer to elem_data of element s of datum d
void *get_datum_elem_data(char *d, char *s)
{
	conditions_list *list;
	if(d==NULL)
		return NULL;
	if(s==NULL)
		return NULL;

	for(list=(get_datum(d))->conditions_head;list!=NULL;list=list->next)
	{
		if(strcmp(list->datum_condition.elem_name,s)==0)
			return list->datum_condition.elem_data;
	}

	return NULL;
}

conditions_list *new_condition_list(condition *c)
{
	conditions_list *list=(conditions_list *)malloc(sizeof(conditions_list));
	list->datum_condition=*c;
	list->next=NULL;
	return list;
}

conditions_list *append_condition(conditions_list *list,condition *c)
{
	conditions_list *tmp;
	
	for(tmp=list;tmp->next!=NULL;tmp=tmp->next);
	tmp->next=(conditions_list *)malloc(sizeof(conditions_list));
	tmp=tmp->next;
	tmp->datum_condition=*c;
	tmp->next=NULL;
	return list;
}


datum_list *datum_list_head=NULL;
datum_list *datum_list_tail=NULL;

void insert_datum_ir(int type,char *name, conditions_list *list)
{
	datum_list *iter;


	if(datum_list_head==NULL)
	{
		datum_list_head=(datum_list *)malloc(sizeof(datum_list));
		datum_list_head->datum.type=type;
		datum_list_head->datum.identifier=(char *)malloc(strlen(name)+1);
		strcpy(datum_list_head->datum.identifier,name);
		datum_list_head->datum.conditions_head=list;
		datum_list_head->datum.state=STATE_UNBORN;
		datum_list_head->next=NULL;
		datum_list_tail=datum_list_head;
	}
	else
	{
		//Check for repetition
		for(iter=datum_list_head;iter!=NULL;iter=iter->next)
		{
			if(strcmp(iter->datum.identifier,name)==0)
			{
				errors++;
				printf("Error: Redefinition of datum %s.\n",name);
				return;
			}
		}

		datum_list_tail->next=(datum_list *)malloc(sizeof(datum_list));
		datum_list_tail=datum_list_tail->next;
		datum_list_tail->datum.type=type;
		datum_list_tail->datum.identifier=(char *)malloc(strlen(name)+1);
		strcpy(datum_list_tail->datum.identifier,name);
		datum_list_tail->datum.conditions_head=list;
		datum_list_tail->datum.state=STATE_UNBORN;
		datum_list_tail->next=NULL;
	}
}


datum_ir *get_datum(char *s)
{
	datum_list *iter;
	for(iter=datum_list_head;iter!=NULL;iter=iter->next)
	{
		if(strcmp(iter->datum.identifier,s)==0)
			return &(iter->datum);
	}
	return NULL;
}


//returns true if term is solvable
int term_solvable(struct term_data *term)
{
	switch(term->type)
	{
	case(TERM_MEMBER):
	{
		if(datum_has_element(get_datum((char *)(term->data1)),(char *)(term->data2)))
		{
			return 1;
		}
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
		if((term->data1==NULL)||(term->data2==NULL))
			return 0;
		if(!term_solvable((struct term_data *)(term->data1)))
			return 0;
		if(!term_solvable((struct term_data *)(term->data2)))
			return 0;
		return 1;
	} break;
	default:
	{
		return 0;
	} break;
	}
	return 0;
}

void static_check_condition(condition *c, datum_ir *datum)
{
	switch(c->type)
	{
	case(CONDITION_VOID):
	{
		//no errors, always valid
		return;
	} break;
	case(CONDITION_NUM_CONST):
	{
		//Only valid if type is INTEGER
		if(c->elem_type!=ELEMENT_INTEGER)
		{
			printf("Error: type mismatch in datum \"%s\" statement.\n",datum->identifier);
			errors++;
			return;
		}
	} break;
	case(CONDITION_IDENTIFIER):
	{
		//Only valid if identifier exists
		if(get_datum(c->datum_dependency)==NULL)
		{
			printf("Error: request for non-existing datum \"%s\" in datum \"%s\".\n",c->datum_dependency,datum->identifier);
			errors++;
			return;
		}
	} break;
	case(CONDITION_INPUT):
	{
		//only valid if datum is of type FINPUT
		if(datum->type!=TYPE_FINPUT)	
		{
			printf("Error: input statement in datum \"%s\" of non \"finput\" type.\n",datum->identifier);
			errors++;
			return;
		}
	} break;
	case(CONDITION_TERM):
	{
		//check if term is solvable
		//if yes, link to data
		if(!term_solvable(c->term))
		{
			printf("Error: statement in datum \"%s\" is not solvable.\n",datum->identifier);
			errors++;
			return;
		}
	} break;
	default: 
	{
		printf("Error: unknow type in datum \"%s\" statement.\n",datum->identifier);
		errors++;
		return;
	} break;
	}
}


void static_check_datum(datum_ir *datum)
{
	conditions_list *list=datum->conditions_head;
	condition *c;

	for(c=&(list->datum_condition);list!=NULL;list=list->next,c=&(list->datum_condition))
	{
		static_check_condition(c,datum);
	}
}

void static_checks()
{
	datum_list *iter;
	for(iter=datum_list_head;iter!=NULL;iter=iter->next)
	{
		static_check_datum(&(iter->datum));
	}
	return;
}


struct term_data *create_term(int type, void *data1, void *data2)
{
	struct term_data *t=(struct term_data *)malloc(sizeof(struct term_data));
	
	//if(type==TERM_NUM_CONST) printf("\n%d\n",*(int *)data1);

	switch(type)
	{
	case(TERM_MEMBER):
	{
		//data1 and data2 are datum and member identifier strings, respectively
		t->type=type;
		t->data1=(void *)((char *)malloc(strlen((char *)data1)+1));
		strcpy((char *)(t->data1),(char *)data1);
		t->data2=(void*)((char *)malloc(strlen((char *)data2)+1));
		strcpy((char *)(t->data2),(char *)data2);
		return t;
		
	} break;
	case(TERM_NUM_CONST):
	{
		t->type=type;
		t->data1=(void *)((int *)malloc(sizeof(int)));
		*(int *)(t->data1)=*(int *)data1;
		return t;
	} break;
	case(TERM_TIMES):
	case(TERM_DIVISION):
	case(TERM_MINUS):
	case(TERM_PLUS):
	{
		t->type=type;
		t->data1=data1;
		t->data2=data2;
		return t;
	} break;
	default: 
	{
		printf("Error: unknown term type.\n");
		errors++;
		return t;
	} break;
	}

	return t;
}



//Debug functions
void dbg_print_conditions(datum_ir d)
{
	conditions_list *tmp;
	for(tmp=d.conditions_head;tmp!=NULL;tmp=tmp->next)
	{
		printf("\tcondition %d\n",tmp->datum_condition.type);
	}
}

void dbg_print_ir()
{
	datum_list *iter=datum_list_head;
	printf("Datum list:\n");
	while(iter!=NULL)
	{
		printf("Type %d name %s\n",iter->datum.type,iter->datum.identifier);
		dbg_print_conditions(iter->datum);
		iter=iter->next;
	}
}





