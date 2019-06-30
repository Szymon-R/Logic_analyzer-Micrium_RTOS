#include "debug_functions.h"
#include "log.h"
void error_handling_other(gen_errors error)
{
	switch(error)
	{
		case null_ptr:
			break;
		case buffer_overflow:
			break;
		case empty_string:
			break;
	 case data_transmission_error:
			break;
		default:
			break;
	}
	while(1)
	{
		;
	}
}
void error_handling_os(uint8_t error)
{
	while(1)
	{
		;
	}
}
