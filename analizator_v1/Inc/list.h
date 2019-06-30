#ifndef LIST_GUARD
#define LIST_GUARD



#define circ_buffer_size 20
#define max_log_length 70
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>


typedef struct logg
{
	char data[max_log_length];
	char priority;
}logg;

typedef struct node
{
	uint8_t ID;
	struct node* next;
	struct logg data;
}node;



void list_init(void);
void list_move(int number);
node* list_get_current(void);
void list_set_current(node* element);
void list_insert_element(logg* data);
void list_move(int number);

#endif
