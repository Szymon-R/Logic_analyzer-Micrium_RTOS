#ifndef PARSER_GUARD
#define PARSER_GUARD

#include <stdio.h>
#include <time.h>
#include "log.h"



#define max_command_length 20



typedef struct parser_config
{
	uint8_t data_counter;
	char command[max_command_length];
	char buffer;
	uint8_t command_length;
}parser_config;

typedef struct command
{
	char text[5];
	uint8_t (*function_handler)(parser_config*);
}command;

void init_parser();
	
int process_command(parser_config* pc);
int get_int_prameter(parser_config* pc, uint8_t which);
void clear_command(parser_config* pc);


#endif


