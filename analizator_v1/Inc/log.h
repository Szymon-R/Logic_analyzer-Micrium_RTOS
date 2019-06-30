

#ifndef LOG_GUARD
#define LOG_GUARD

#define arg_size 12
#include "list.h"

typedef struct log_config
{
	uint8_t print_logs;
	uint8_t enable_high;
	uint8_t enable_medium;
	uint8_t enable_low;
}log_config;

typedef enum error_codes
{
	NO_ERROR
	
}error_codes;


int form_log(uint8_t priority,const int num,const char* string,va_list valist);
int log_high(const int num,const char* string,...);
int log_medium(const int num,const char* string,...);
int log_low(const int num,const char* string,...);
void print_log(const logg* temp);
void init_logs(void);
void print_command_output(const char* data);
#endif

