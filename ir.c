#include"epsi.h"

extern int errors;

condition *new_condition(int type, int elem_type, char *d_name, int num_const)
{
	condition *cond=(condition *)malloc(sizeof(condition));
	cond->type=type;
	cond->elem_type=elem_type;
	cond->datum_dependency=d_name;

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





