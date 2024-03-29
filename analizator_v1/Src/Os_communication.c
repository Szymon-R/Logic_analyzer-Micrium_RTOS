

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
#define buffer_size 2000u


extern OS_ERR os_err;
OS_TCB Task_CaptureData_TCB;

CPU_STK Task_CaptureData_STK[Task_captureData_stack_size];
extern TIM_HandleTypeDef htim2;
uint8_t Signal_task_capture_data_stop;
uint32_t data_buffer_counter=0;
uint32_t data_buffer[buffer_size];//TO DO: move it into function

void Task_captureData_task(void* p_arg)
{
	memset(data_buffer,0,sizeof(int)*buffer_size);
	GPIO_PinState last;
	GPIO_PinState current;
	while(DEF_TRUE)
	{
		OSTimeDlyHMSM(0,0,0,2,OS_OPT_TIME_HMSM_STRICT,&os_err);
		HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
		HAL_TIM_Base_Start_IT(&htim2);
		
		last=HAL_GPIO_ReadPin(INPUT_GPIO_Port,INPUT_Pin);
		while(DEF_TRUE)
		{
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
				correct_data(data_buffer,buffer_size);
				send_data(data_buffer,buffer_size);
				//__HAL_TIM_SetCounter(&htim2,0);
				HAL_Delay(1000);
			}
		}
	}
}  

void Task_captureData_init(void(*function)(void* p_arg))
{
	OSInit(&os_err);
	ASSERT_NO_OSERR(os_err);
	Signal_task_capture_data_stop=0;
	OSTaskCreate(&Task_CaptureData_TCB,//task control block
								"Capture data",//string for debugging
								function,//pointer to the task to be perforemed
								(void*)0,//pasiing parameters to the task
								Task_captureData_priority,//task priority
								Task_CaptureData_STK,//pointer to task's stack
								0u,//
								Task_captureData_stack_size,//size of the stack ie elements. 
								0,//message queue
								0,//time quanata for this task in round robin
								0,//extension of TCB, for otherelements like FPU
								OS_OPT_TASK_STK_CHK,//task specific options
								&os_err);//error handler
	ASSERT_NO_OSERR(os_err);								
	OSStart(&os_err);
	ASSERT_NO_OSERR(os_err);								
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