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

	UartMessageSimplified.h

  Abstract:

	Uart protocol frame data structures (simplified version) header file
*/

#ifndef UART_MESSAGE_SIMPLIFIED_H
#define UART_MESSAGE_SIMPLIFIED_H

#include <stdio.h>


//-----------------------------------------------------------------------------
//  Define simplified protocol version variables
//-----------------------------------------------------------------------------
#define SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE		3   // in bytes
#define SIMPLIFIED_UART_PROTOCOL_OVERHEAD_SIZE		2   // in bytes ($ and #)
#define SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE	(SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE + SIMPLIFIED_UART_PROTOCOL_OVERHEAD_SIZE)

#endif