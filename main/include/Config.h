/* 
	Copyright(C) 2018 The University of Texas at Dallas
	Developed By: Muhammet Emin Yanik
	Advisor: Prof. Murat Torlak
	Department of Electrical and Computer Engineering

	This work was supported by the Semiconductor Research Corporation (SRC) task 2712.029
	through The University of Texas at Dallas' Texas Analog Center of Excellence (TxACE).

	Redistributions and use of source must retain the above copyright notice
	Redistributions in binary form must reproduce the above copyright notice
*/

/*
  Module Name:

	Config.h

  Abstract:

	The configuration header file
*/

#ifndef CONFIG_H
#define CONFIG_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
// Common thread/task parameters
//-----------------------------------------------------------------------------
#define DEFAULT_TASK_STACK_SIZE_WORDS       1 * 1024
#define DEFAULT_TASK_STACK_SIZE_BYTES       DEFAULT_TASK_STACK_SIZE_WORDS * sizeof(uint32_t)

//-----------------------------------------------------------------------------
// If the advanced protocol is used, comment this line
// Otherwise, the simplified command protocol will be used
//-----------------------------------------------------------------------------
#define SIMPLIFIED_PROTOCOL_VERSION

//-----------------------------------------------------------------------------
// If the test mode is active, comment out this line
// Then the pulse counter will work based on the internally created pulses
//-----------------------------------------------------------------------------
// #define INTERNAL_TEST_MODE

/* The data type to pass events from the Uart task to the radar trigger task */
typedef struct {
    int command;  	// the command for the Radar trigger task
    uint32_t data; 	// the data for the Radar trigger task
} uart_evt_t;

enum eUART_RADAR_TRIGGER_COMMAND_SET {
	UART_RADAR_TRIGGER_COMMAND = 1,
	UART_DESIRED_NUM_TRIGGER_COMMAND,
	UART_CLEAR_NUM_TRIGGER_COMMAND,
};

#endif