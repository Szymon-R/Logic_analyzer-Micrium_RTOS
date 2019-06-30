#ifndef TASK_CAPTUREDATA_GUARD
#define TASK_CAPTUREDATA_GUARD

#include "stm32f4xx.h"                  // Device header
#include "os.h"                         // Micrium.Micrium::RTOS:uC/OS Kernel
#include "stm32f4xx_hal.h"
#include "gpio.h"

void Task_captureData_init(void(*function)(void* p_arg));
void Task_captureData_task(void* p_arg);
void send_data(uint32_t* data,size_t size);
void correct_data(uint32_t* data,size_t size);
void correct_data(uint32_t* data,size_t size);


#endif
