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

	Uart.h

  Abstract:

	The header file of the Uart handler
*/

#ifndef UART_H
#define UART_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#include <Config.h>


// Uart RX and TX buffer size
#define UART_BUFFER_SIZE 	1024

/* A queue to handle Uart radar trigger events */
xQueueHandle uart_evt_queue;

/* 
	The data type to pass events from the Uart task
	to the radar trigger task.
 */
typedef struct {
    int command;  	// the command for the Radar trigger task
    uint32_t data; 	// the data for the Radar trigger task
} uart_evt_t;

enum eUART_RADAR_TRIGGER_COMMAND_SET {
	UART_RADAR_TRIGGER_COMMAND = 1,
	UART_DESIRED_NUM_TRIGGER_COMMAND,
	UART_COMPLETE_NUM_TRIGGER_COMMAND,
};

// Initialize the UART Communication
void uartInitialize(void);

#endif