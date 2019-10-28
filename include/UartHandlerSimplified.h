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

	UartHandlerSimplified.c

  Abstract:

	Uart protocol handler (simplified version) header file
*/


#ifndef UART_HANDLER_SIMPLIFIED_H
#define UART_HANDLER_SIMPLIFIED_H

#include <UartMessageSimplified.h>


//-----------------------------------------------------------------------------
// handle the post buffer (simplified version)
// prepare the reply buffer (simplified version)
//-----------------------------------------------------------------------------
void uartHandleBufferSimplified(
    const uint8_t* pPostBuffer,         // Pointer to the post buffer
    uint32_t postSizeInBytes,           // Post buffer size
    uint8_t* pReplyBuffer,              // Pointer to the reply buffer
    uint32_t replySizeInBytes,          // Reply buffer size
    uint32_t* pNumReplyBytesWritten);   // Pointer to the number of reply bytes written


//-----------------------------------------------------------------------------
// handle the radar trigger command
//-----------------------------------------------------------------------------
void handleRadarTriggerCommand(void);

//-----------------------------------------------------------------------------
// handle the set desired number of radar trigger command
//-----------------------------------------------------------------------------
void handleSetDesiredNumberOfTriggerCommand(const uint8_t* pCommand,
                                		uint32_t postSizeInBytes);

//-----------------------------------------------------------------------------
// handle the clear number of radar trigger command
//-----------------------------------------------------------------------------
void handleClearNumberOfTriggerCommand(void);

//-----------------------------------------------------------------------------
// handle the set pulse count command
//-----------------------------------------------------------------------------
void handleSetPulseCountCommand(const uint8_t* pCommand,
                                uint32_t postSizeInBytes);

//-----------------------------------------------------------------------------
// handle the reset command
//-----------------------------------------------------------------------------
void handleResetPcntCommand(void);

//-----------------------------------------------------------------------------
// handle the pause command
//-----------------------------------------------------------------------------
void handlePausePcntCommand(void);

//-----------------------------------------------------------------------------
// handle the resume command
//-----------------------------------------------------------------------------
void handleResumePcntCommand(void);

//-----------------------------------------------------------------------------
// handle the set number of measurements command
//-----------------------------------------------------------------------------
void handleSetNumMeasurementCommand(const uint8_t* pCommand,
                                	uint32_t postSizeInBytes);

#endif