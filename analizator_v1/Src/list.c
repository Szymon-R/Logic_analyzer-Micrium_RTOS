#include "list.h"


node circ_buffer[circ_buffer_size];
node* current_element;

void list_init()
{
	node* head=circ_buffer;
	for(int i=0; i<circ_buffer_size-1;++i)
	{
		circ_buffer[i].next=&(circ_buffer[i+1]);
		circ_buffer[i].ID=i+1;
		memset(circ_buffer[i].data.data,0,max_log_length);
	}
	
	circ_buffer[circ_buffer_size-1].next=head;
	circ_buffer[circ_buffer_size-1].ID=circ_buffer_size;
	memset(circ_buffer[circ_buffer_size-1].data.data,0,max_log_length);
	current_element=head;
}

node* list_get_current()
{
	return current_element;
}

void list_move(int number)
{
	node* element=list_get_current();
	for(int i=0; i<number;++i)
	{
		element=element->next;
	}
	list_set_current(element);
}

void list_set_current(node* element)
{
	current_element=element;
}
node* list_find_item(node* element, uint8_t ID)
{
	node* head=list_get_current();
	for(int i=0; i<circ_buffer_size;++i)
	{
		if(head->ID==ID)
			return head;
		head=head->next;
	}
	return NULL;
}

void list_insert_element(logg* data)
{
	node* element=list_get_current();
	memset(element->data.data,0,max_log_length);
	strcpy(element->data.data,data->data);
	element->data.priority=data->priority;
	list_move(1);
}
