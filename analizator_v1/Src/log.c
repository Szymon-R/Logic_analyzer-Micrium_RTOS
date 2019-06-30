#include "log.h"
#include "debug_functions.h"
#include "usart.h"
extern log_config lc;


int form_log(uint8_t priority,const int num,const char* string,va_list valist)
{  
  int i = 0, j=0;
	char buff[max_log_length]={0};
 	char arg[arg_size];
	int limit=max_log_length-num*arg_size+2*num;

	if(num*arg_size+2*num>max_log_length)
		return 0;
	while (string && string[i]&& i<limit)
  {
		if(string[i] == '%')
		{
			memset(arg,0,arg_size);
			i++;
			switch (string[i]) 
			{
				case 'c': 
				{
					buff[j] = (char)va_arg( valist, int );
					j++;
					break;
				}
				case 'd': 
				{
					sprintf(arg,"%d",va_arg( valist, int ));
					strcpy(&buff[j], arg);
					j += strlen(arg);
					break;
				}
				case 'f': 
				{  
					snprintf(arg, arg_size, "%f", va_arg( valist,double ));
					strcpy(&buff[j], arg);
					j += strlen(arg);
					break;
				}
			}
		} 
		else 
		{
			buff[j] =string[i];
			j++;
  	}
		i++;
	} 
	logg temp;
	buff[max_log_length-1]='\0';
	memset(temp.data,0,max_log_length);
	strcpy(temp.data,buff);
	temp.priority=priority;
	list_insert_element(&temp);
	print_log(&temp);
	return 1;
}




int log_medium(const int num,const char* string,...)
{
	if(!lc.enable_medium)
		return 0;
	va_list valist;
	va_start( valist, string );
	form_log(2,num,string,valist);
	va_end( valist);
  return 1;
}

int log_low(const int num,const char* string,...)
{
	if(!lc.enable_low)
		return 0;
	va_list valist;
	va_start( valist, string );
	form_log(1,num,string,valist);
	va_end( valist);
  return 1;
}

int log_high(const int num,const char* string,...)
{
	if(!lc.enable_high)
		return 0;
	va_list valist;
	va_start( valist, string );
	form_log(3,num,string,valist);
	va_end( valist);
  return 1;
}


void init_logs()
{
	lc.print_logs=1;
	lc.enable_high=1;
	lc.enable_low=1;
	lc.enable_medium=1;
}
void print_log(const logg* temp)
{

	if(lc.print_logs==1)
	{

		switch(temp->priority)
		{
			case 1:
				print_uart("<HIGH: ");
			break;
			case 2:
				print_uart("<MEDIUM: ");
			break;
		 case 3:
			 print_uart("<LOW: ");
			break;
		 default:
			 print_uart("<----------------");
		}

		print_uart(temp->data);
		print_uart(">\n");


	}
		
}