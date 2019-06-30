#ifndef TASK_DEBUG_FUNCTIONS_GUARD
#define TASK_DEBUG_FUNCTIONS_GUARD

#include "os.h"                         // Micrium.Micrium::RTOS:uC/OS Kernel
#include <stdint.h>
#include <stdio.h>

typedef enum  gen_errors
{
	null_ptr,
	buffer_overflow,
	empty_string,
	data_transmission_error
}gen_errors;

void error_handling_other(gen_errors error);
void error_handling_os(uint8_t error);
#define ASSERT_PTR(x) if (!(x)) error_handling_other((gen_errors)0);
#define ASSERT_STR_SIZE(x) if ((!x)) error_handling_other((gen_errors)2);
#define ASSERT_NO_OSERR(x) if ((x)) error_handling_os((OS_ERR)x);

#endif
