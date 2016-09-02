#include"epsi.h"

struct datum_list
{
	datum_ir datum;
	struct datum_list *next;
};
typedef struct datum_list datum_list;

datum_list *datum_list_head=NULL;
datum_list *datum_list_tail=NULL;

void insert_datum_ir(int type,char *name)
{
	if(datum_list_head==NULL)
	{
		datum_list_head=(datum_list *)malloc(sizeof(datum_list));
		datum_list_head->datum.type=type;
		datum_list_head->datum.identifier=(char *)malloc(strlen(name)+1);
		strcpy(datum_list_head->datum.identifier,name);
		datum_list_head->next=NULL;
		datum_list_tail=datum_list_head;
	}
	else
	{
		datum_list_tail->next=(datum_list *)malloc(sizeof(datum_list));
		datum_list_tail=datum_list_tail->next;
		datum_list_tail->datum.type=type;
		datum_list_tail->datum.identifier=(char *)malloc(strlen(name)+1);
		strcpy(datum_list_tail->datum.identifier,name);
		datum_list_tail->next=NULL;
	}
}


//Debug functions
void dbg_print_ir()
{
	datum_list *iter=datum_list_head;
	printf("Datum list:\n");
	while(iter!=NULL)
	{
		printf("Type %d name %s\n",iter->datum.type,iter->datum.identifier);
		iter=iter->next;
	}
}
