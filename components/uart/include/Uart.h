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

#include "Config.h"
#include "esp_system.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"


// Uart RX and TX buffer size
#define UART_BUFFER_SIZE 	1024

// Define following to set the UART debug port (Use UART_NUM_2)
// #define UART_DEBUG_MODE

#ifdef UART_DEBUG_MODE
	#define UART_DATA_RXD_PIN (GPIO_NUM_16)
	#define UART_DATA_TXD_PIN (GPIO_NUM_17)
#endif

// Initialize the UART Communication
void uartInitialize(void);

#endif