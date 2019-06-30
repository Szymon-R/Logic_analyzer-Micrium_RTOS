#include "parser.h"

#define commands_number 14
/*char lmgl[5]="lmgl";

char suty[5]="suty";
char sutm[5]="sutm";
char sutd[5]="sutd";
char suth[5]="suth";
char sumi[5]="sumi";

char swty[5]="swty";
char swtm[5]="swtm";
char swtd[5]="swtd";
char swth[5]="swth";
char swmi[5]="swmi";*/

command commands[commands_number];

void init_parser()
{
	strcpy(commands[0].text, "lmgl");
	commands[0].function_handler=&lmgl_handling;
	
	strcpy(commands[1].text, "suty");
	commands[1].function_handler=&suty_handling;
	
	strcpy(commands[2].text, "sutm");
	commands[2].function_handler=&sutm_handling;	
	
	strcpy(commands[3].text, "sutd");
	commands[3].function_handler=&sutd_handling;	
	
	strcpy(commands[4].text, "suth");
	commands[4].function_handler=&suth_handling;	
	
	strcpy(commands[5].text, "sumi");
	commands[5].function_handler=&sumi_handling;	
	
	strcpy(commands[6].text, "swty");
	commands[6].function_handler=&swty_handling;	
	
	strcpy(commands[7].text, "swtm");
	commands[7].function_handler=&swtm_handling;	
	
	strcpy(commands[8].text, "swtd");
	commands[8].function_handler=&swtd_handling;	
	
	strcpy(commands[9].text, "swth");
	commands[9].function_handler=&swth_handling;	
	
	strcpy(commands[10].text, "swmi");
	commands[10].function_handler=&swmi_handling;	
	
	strcpy(commands[11].text, "gwut");
	commands[11].function_handler=&gwut_handling;	
	
	strcpy(commands[12].text, "gctt");
	commands[12].function_handler=&gctt_handling;	
	
	strcpy(commands[13].text, "rwut");
	commands[13].function_handler=&rwut_handling;	
}
/*int process_command(parser_config* pc)
{
	uint8_t comma=0;
	char text[5];
	for(comma=0; comma<max_command_length-1;++comma)
	{
		if(pc->command[comma]==',')
		break;
	}
	if(comma!=pc->command_length)
	{
		log_low(0,"Nieprawidlowa dlugosc czesci tekstowej komendy");
		return 0;
	}
	for(int i=0; i<pc->command_length;++i)
	{
		text[i]=pc->command[i];
	}
	text[4]='\0';
	if(!strcmp(text, lmgl)) 
	{
		 if(lmgl_handling(pc)!=1)
			 return 0;
	}
	else if(!strcmp(text, suty)) 
	{
		 if(suty_handling(pc)!=1)
			 return 0;
	}
	
	else if(!strcmp(text, sutm)) 
	{
		 if(sutm_handling(pc)!=1)
			 return 0;
	}
	
	else if(!strcmp(text, sutd)) 
	{
		 if(sutd_handling(pc)!=1)
			 return 0;
	}
	
	else if(!strcmp(text, suth)) 
	{
		 if(suth_handling(pc)!=1)
			 return 0;
	}
	
	else if(!strcmp(text, sumi)) 
	{
		 if(sumi_handling(pc)!=1)
			 return 0;
	}
	
	else if(!strcmp(text, swty)) 
	{
		 if(swty_handling(pc)!=1)
			 return 0;
	}
	
	else if(!strcmp(text, swtm)) 
	{
		 if(swtm_handling(pc)!=1)
			 return 0;
	}
	
	else if(!strcmp(text, swtd)) 
	{
		 if(swtd_handling(pc)!=1)
			 return 0;
	}
	
	else if(!strcmp(text, swth)) 
	{
		 if(swth_handling(pc)!=1)
			 return 0;
	}
	
	else
	{
		log_low(0,"Nieprawidlowa czesc tekstowa komendy");
		return 0;
	}
	return 1;
}*/

int process_command(parser_config* pc)
{
	uint8_t comma=0;
	char text[5];
	for(comma=0; comma<max_command_length-1;++comma)
	{
		if(pc->command[comma]==',')
		break;
	}
	if(comma!=pc->command_length)
	{
		log_low(0,"Nieprawidlowa dlugosc czesci tekstowej komendy");
		return 0;
	}
	for(int i=0; i<pc->command_length;++i)
	{
		text[i]=pc->command[i];
	}
	text[4]='\0';

	for(int i=0; i<commands_number;++i)
	{
		if(!strcmp(text, commands[i].text))
		{
			commands[i].function_handler(pc);
			return 1;
		}
	}

	log_low(0,"Nieprawidlowa czesc tekstowa komendy");
	return 0;
}


int get_int_prameter(parser_config* pc, uint8_t which)
{
	char buffer[12]={0};
	if(which==0)
	{
		log_low(0,"Nie mozna zadac parametru 0");
		return 0;
	}
	
	uint8_t previous=0;
	uint8_t next=0;
	uint8_t counter=0;
	for(int i=0; i<max_command_length;++i)
	{
		if(pc->command[i]==',')
			++counter;
		if(previous==0&&counter==which)
			previous=i;
		if(next==0&&counter==(which+1))
		{
			next=i;
			break;
		}
	}
	int space=next-previous;
	if((!(previous&&next))||space<=0)
	{
		log_low(0,"Cos poszlo nie tak przy wyszukiwaniu parametru");
		return 0;
	}
	for(int i=0;i<space-1;++i)
	{
		buffer[i]=pc->command[previous+i+1];
	}
	int l = strtol(buffer,NULL,10);
	return l;
}


uint8_t lmgl_handling(parser_config* pc)
{
	int start;
	start=get_int_prameter(pc,1);
	if(start!=-1)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie lmgl: ",start);
		return 0;
	}
	node* head=list_get_current();
	if(start==-1)
	{
		for(int i=0; i<circ_buffer_size;++i)
		{
			list_move(1);
		//	if(list_get_current()->data.time!=0)
			print_log(&(list_get_current()->data));
		}
		list_set_current(head);
		return 1;
	}
	return 0;
}

uint8_t suty_handling(parser_config* pc)//set unix time year
{
	int start;
	start=get_int_prameter(pc,1);
	if(start==0||start<1900)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie suty: ",start);
		return 0;
	}
	tm temp_time=get_current_time();
	temp_time.tm_year=start-1900;
	time_t t=mktime(&temp_time);
	set_time(t);
	return 1;
}

uint8_t sutm_handling(parser_config* pc)//set unix time month
{
	int start;
	start=get_int_prameter(pc,1);
	if(start<=0||start>12)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie sutm: ",start);
		return 0;
	}
	tm temp_time=get_current_time();
	temp_time.tm_mon=start-1;
	time_t t=mktime(&temp_time);
	set_time(t);
	return 1;
}

uint8_t sutd_handling(parser_config* pc)//set unix time day
{
	int start;
	start=get_int_prameter(pc,1);
	if(start<=0||start>31)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie sutd: ",start);
		return 0;
	}
	tm temp_time=get_current_time();
	temp_time.tm_mday=start;
	time_t t=mktime(&temp_time);
	set_time(t);
	return 1;
}

uint8_t suth_handling(parser_config* pc)//set unix time hour
{
	int start;
	start=get_int_prameter(pc,1);
	if(start<=0||start>24)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie suth: ",start);
		return 0;
	}
	tm temp_time=get_current_time();
	temp_time.tm_hour=start;
	time_t t=mktime(&temp_time);
	set_time(t);
	return 1;
}

uint8_t sumi_handling(parser_config* pc)//set unix time minute
{
	int start;
	start=get_int_prameter(pc,1);
	if(start<=0||start>60)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie sumi: ",start);
		return 0;
	}
	tm temp_time=get_current_time();
	temp_time.tm_min=start;
	time_t t=mktime(&temp_time);
	set_time(t);
	return 1;
}

uint8_t swty_handling(parser_config* pc)//set wake up time year
{
	int start;
	start=get_int_prameter(pc,1);
	if(start==0||start>17)//end of unix time in 2038.
	{
		log_low(1,"Nieprawidlowy parametr w komendzie swty: ",start);
		return 0;
	}
		tm temp_time;
	time_t t=0;
	temp_time=*localtime(&t);
	temp_time.tm_year=start;
	set_wakeup_time(temp_time);
	return 1;
}

uint8_t swtm_handling(parser_config* pc)//set wake up time month
{
	int start;
	start=get_int_prameter(pc,1);
	if(start==0||start>12)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie swtm: ",start);
		return 0;
	}
		tm temp_time;
	time_t t=0;
	temp_time=*localtime(&t);
	temp_time.tm_mon=start;
	set_wakeup_time(temp_time);
	return 1;
}

uint8_t swtd_handling(parser_config* pc)//set wake up time day
{
	int start;
	start=get_int_prameter(pc,1);
	if(start==0||start>31)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie swtd: ",start);
		return 0;
	}
		tm temp_time;
	time_t t=0;
	temp_time=*localtime(&t);
	temp_time.tm_mday=start;
	set_wakeup_time(temp_time);
	return 1;
}

uint8_t swth_handling(parser_config* pc)//set wake up time hour
{
	int start;
	start=get_int_prameter(pc,1);
	if(start==0||start>24)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie swtm: ",start);
		return 0;
	}
		tm temp_time;
	time_t t=0;
	temp_time=*localtime(&t);
	temp_time.tm_hour=start;
	set_wakeup_time(temp_time);
	return 1;
}

uint8_t swmi_handling(parser_config* pc)//set wake up time minute
{
	int start;
	start=get_int_prameter(pc,1);
	if(start==0||start>60)
	{
		log_low(1,"Nieprawidlowy parametr w komendzie swtm: ",start);
		return 0;
	}
	tm temp_time;
	time_t t=0;
	temp_time=*localtime(&t);
	temp_time.tm_min=start;
	set_wakeup_time(temp_time);
	return 1;
}

uint8_t gwut_handling(parser_config* pc)//get wake up timer
{
	tm current_time=get_current_time();
	tm next_wake=get_next_wakeup();
	time_t t=mktime(&current_time)+mktime(&next_wake);
	tm temp=*localtime(&t);
  char buffer[26];
  strftime(buffer, 26, "%Y-%m-%d %H:%M", &temp);
  puts(buffer);
	print_command_output(buffer);
	return 1;
}

uint8_t gctt_handling(parser_config* pc)//get current time
{
	tm temp=get_current_time();
  char buffer[26];
  strftime(buffer, 26, "%Y-%m-%d %H:%M", &temp);
  puts(buffer);
	print_command_output(buffer);
	return 1;
}

uint8_t rwut_handling(parser_config* pc)//get wake up timer
{
	reset_wakeup_time();
	return 1;
}
void clear_command(parser_config* pc)
{
	memset(pc->command,0,max_command_length);
	pc->data_counter=0;
}
