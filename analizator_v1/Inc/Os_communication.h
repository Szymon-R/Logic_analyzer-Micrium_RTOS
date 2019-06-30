#ifndef OS_COMMUNICATION_GUARD
#define OS_COMMUNICATION_GUARD

#include "stm32f4xx.h"                  // Device header
#include "os.h"                         // Micrium.Micrium::RTOS:uC/OS Kernel
#include "stm32f4xx_hal.h"
#include "gpio.h"

#define buffer_size 2000u


#define Task_sendData_stack_size 128u
#define Task_sendData_priority 3u
#define Task_sendData_stack_size 128u

#define Task_captureData_stack_size 128u
#define Task_captureData_priority 1u
#define Task_capture_data_stop 0x01

extern OS_FLAG_GRP  DataSending;
#define DataReady 	0x01
#define DataSent  	0x02

extern OS_MUTEX  data_buffer_mutex;
extern CPU_TS    data_buffer_mutex_ts;

extern OS_ERR os_err;

#endif
