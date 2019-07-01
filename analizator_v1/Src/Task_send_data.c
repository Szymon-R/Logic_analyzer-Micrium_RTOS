#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>

#include "Task_send_data.h"
#include "debug_functions.h"
#include "main.h"
#include "usart.h"
#include "log.h"
#include "Os_communication.h"


OS_TCB Task_SendData_TCB;
OS_FLAG_GRP  DataSending;
OS_MUTEX  data_buffer_mutex;
CPU_TS    data_buffer_mutex_ts;

CPU_STK Task_SendData_STK[Task_sendData_stack_size];
extern TIM_HandleTypeDef htim2;
uint32_t data_buffer_copy[buffer_size];


void Task_sendData_task(void* p_arg)
{
	uint32_t* data_buffer=0;
	memset(data_buffer_copy,0,buffer_size*sizeof(int));
	while(DEF_TRUE)
	{
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&os_err);
		//OSFlagPend(&DataSending,DataReady,0,OS_OPT_PEND_NON_BLOCKING, (CPU_TS *)0,&os_err);
		//ASSERT_NO_OSERR(os_err);
		data_buffer=OSQPend(&data_pass_queue,0,OS_OPT_PEND_BLOCKING,(OS_MSG_SIZE*)sizeof(int*),(CPU_TS*)0,&os_err);
		ASSERT_NO_OSERR(os_err);
		OSMutexPend(&data_buffer_mutex,0, OS_OPT_PEND_BLOCKING,&data_buffer_mutex_ts,&os_err);	
		OSMutexPost(&data_buffer_mutex,OS_OPT_POST_NO_SCHED,&os_err);
		copy_data_buffer(data_buffer,data_buffer_copy);
		OSMutexPost (&data_buffer_mutex,OS_OPT_POST_NONE,&os_err);
		
		correct_data(data_buffer_copy,buffer_size);
		send_data(data_buffer_copy,buffer_size);
		memset(data_buffer_copy,0,buffer_size*sizeof(int));
		OSTaskSuspend((OS_TCB *)0,&os_err);       // Suspend current task    
	}

}  

void Task_sendData_init(void(*function)(void* p_arg))
{
	OSInit(&os_err);
	ASSERT_NO_OSERR(os_err);
	
	
	OSFlagCreate (&DataSending,
                "DataSending",
                (OS_FLAGS) 0,
                &os_err);
	ASSERT_NO_OSERR(os_err);	
	
	OSMutexCreate (&data_buffer_mutex,
                 "data_buffer_mutex",
                 &os_err);
	
	ASSERT_NO_OSERR(os_err);	
	
	OSTaskCreate(&Task_SendData_TCB,//task control block
								"Send data",//string for debugging
								function,//pointer to the task to be perforemed
								(void*)0,//pasiing parameters to the task
								Task_sendData_priority,//task priority
								Task_SendData_STK,//pointer to task's stack
								0u,//
								Task_sendData_stack_size,//size of the stack ie elements. 
								0,//message queue
								50,//time quanata for this task in round robin
								0,//extension of TCB, for otherelements like FPU
								OS_OPT_TASK_STK_CHK,//task specific options
								&os_err);//error handler
	ASSERT_NO_OSERR(os_err);											
}


void copy_data_buffer(uint32_t* source, uint32_t* destination)
{
	for(int i=0; i<buffer_size; ++i)
	{
		destination[i]=source[i];
	}
}

void send_data(uint32_t* data,size_t size)
{
	char buffer[10];
	for(int i=0; i<size;++i)
	{
		memset(buffer,0,10);
		sprintf(buffer,"%d",data[i]);
		print_uart_ln(buffer);
	}
}
void correct_data(uint32_t* data,size_t size)
{
	uint32_t offset=4;//number of machine cycles it takes from signal detection to time capture
	for(int i=0; i<size;++i)
	{
		data[i]-=offset*i;
	}
}

