#ifndef TASK_SENDDATA_GUARD
#define TASK_SENDDATA_GUARD

#include "stm32f4xx.h"                  // Device header
#include "os.h"                         // Micrium.Micrium::RTOS:uC/OS Kernel
#include "stm32f4xx_hal.h"
#include "gpio.h"

void Task_sendData_init(void(*function)(void* p_arg));
void Task_sendData_task(void* p_arg);
void copy_data_buffer(uint32_t* source, uint32_t* destination);
void send_data(uint32_t* data,size_t size);
void correct_data(uint32_t* data,size_t size);

#endif
