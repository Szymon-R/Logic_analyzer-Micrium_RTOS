


#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <string.h>

#include "Task_capture_data.h"
#include "debug_functions.h"
#include "main.h"
#include "usart.h"
#include "log.h"
#include "Os_communication.h"




OS_TCB Task_CaptureData_TCB;

CPU_STK Task_CaptureData_STK[Task_captureData_stack_size];
extern TIM_HandleTypeDef htim2;
uint8_t Signal_task_capture_data_stop;
uint32_t data_buffer_counter=0;//TO DO: move it into function
uint32_t data_buffer[buffer_size];//TO DO: move it into function

OS_Q   data_pass_queue;

void Task_captureData_task(void* p_arg)
{
	memset(data_buffer,0,sizeof(int)*buffer_size);
	GPIO_PinState last;
	GPIO_PinState current;
	while(DEF_TRUE)
	{
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&os_err);
		HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
		HAL_TIM_Base_Start_IT(&htim2);
		
		last=HAL_GPIO_ReadPin(INPUT_GPIO_Port,INPUT_Pin);
		while(DEF_TRUE)
		{
			OSMutexPend(&data_buffer_mutex,0, OS_OPT_PEND_NON_BLOCKING,&data_buffer_mutex_ts,&os_err);	
			while(DEF_TRUE)
			{
				//wait for mutex
				//OSMutexPost (&data_buffer_mutex,OS_OPT_POST_NO_SCHED,&os_err);
				//ASSERT_NO_OSERR(os_err);			
				current=HAL_GPIO_ReadPin(INPUT_GPIO_Port,INPUT_Pin);
				if(current!=last)
				{
					last=current;
					data_buffer[data_buffer_counter]=__HAL_TIM_GetCounter(&htim2);
					
					if(last==GPIO_PIN_RESET)
						data_buffer[data_buffer_counter]|=0x80000000;
					
					data_buffer_counter=((data_buffer_counter + 1) % (buffer_size+1));
					Signal_task_capture_data_stop=(data_buffer_counter==buffer_size);
				}
				if(Signal_task_capture_data_stop)
				{
					Signal_task_capture_data_stop=0;
					//correct_data(data_buffer,buffer_size);
					//send_data(data_buffer,buffer_size);
					break;
				}
			}
			OSMutexPost (&data_buffer_mutex,OS_OPT_POST_NONE,&os_err);
			ASSERT_NO_OSERR(os_err);
			OSQPost(&data_pass_queue,data_buffer,sizeof(uint32_t*),OS_OPT_POST_FIFO,&os_err);
			ASSERT_NO_OSERR(os_err);
			OSFlagPost(&DataSending,DataReady,OS_OPT_POST_FLAG_SET,&os_err);
			ASSERT_NO_OSERR(os_err);
			OSSched();
			//		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&os_err);
		//	OSTaskSuspend((OS_TCB *)0,&os_err);       // Suspend current task    
		}
	}
}  

void Task_captureData_init(void(*function)(void* p_arg))
{
	OSInit(&os_err);
	ASSERT_NO_OSERR(os_err);
	Signal_task_capture_data_stop=0;
	
	
	OSQCreate(&data_pass_queue,
              "Passing data queue",
               5,
              &os_err);   
	 
	OSTaskCreate(&Task_CaptureData_TCB,//task control block
								"Capture data",//string for debugging
								function,//pointer to the task to be perforemed
								(void*)0,//pasiing parameters to the task
								Task_captureData_priority,//task priority
								Task_CaptureData_STK,//pointer to task's stack
								0u,//
								Task_captureData_stack_size,//size of the stack ie elements. 
								0,//message queue
								50,//time quanata for this task in round robin
								0,//extension of TCB, for otherelements like FPU
								OS_OPT_TASK_STK_CHK,//task specific options
								&os_err);//error handler
	ASSERT_NO_OSERR(os_err);								
	//OSStart(&os_err);
	//ASSERT_NO_OSERR(os_err);								
}


