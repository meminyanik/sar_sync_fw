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

	UartHandler.c

  Abstract:

	Uart protocol handler header file
*/


#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include <UartMessage.h>

//-----------------------------------------------------------------------------
// handle the post buffer
// prepare the reply buffer
//-----------------------------------------------------------------------------
void uartHandleBuffer(
    const uint8_t* pPostBuffer,         // Pointer to the post buffer
    uint32_t postSizeInBytes,           // Post buffer size
    uint8_t* pReplyBuffer,              // Pointer to the reply buffer
    uint32_t replySizeInBytes,          // Reply buffer size
    uint32_t* pNumReplyBytesWritten);   // Pointer to the number of reply bytes written

//-----------------------------------------------------------------------------
// the checksum algorithm used is the 8-Bit Fletcher Algorithm (used in the TCP standard)
//-----------------------------------------------------------------------------
void calculateCheckSum(
    const uint8_t* pDataIn,     // Pointer to the byte array
    uint32_t dataLength,        // Length of the byte array
    uint8_t* pChecksum);        // Pointer to the calculated checksum

//-----------------------------------------------------------------------------
// set the header and write into the buffer
//-----------------------------------------------------------------------------
void setUartProtocolHeader(
    uint8_t* pReplyBuffer,          // Pointer to the reply buffer
    uint8_t commandStatus,          // Status of the command
    uint16_t commandId,             // Command ID
    uint16_t payloadLen);           // Payload length

//-----------------------------------------------------------------------------
// get the header from the buffer
//-----------------------------------------------------------------------------
void getUartProtocolHeader(
    const uint8_t* pPostBuffer,         // Pointer to the post buffer
    UartProtocolHeader* pUartHeader);   // Pointer to the header

//-----------------------------------------------------------------------------
// set the footer and write into the buffer
//-----------------------------------------------------------------------------
void setUartProtocolFooter(
    uint8_t* pReplyBuffer,        // Pointer to the reply buffer
    uint16_t payloadLen);         // Payload length

//-----------------------------------------------------------------------------
// set the protocol error response frame
//-----------------------------------------------------------------------------
void setProtocolErrorResponseFrame(
    uint8_t* pReplyBuffer,              // Pointer to the reply buffer
    uint8_t commandStatus,              // Status of the command
    uint32_t* pNumReplyBytesWritten);   // Pointer to the number of reply bytes written

//-----------------------------------------------------------------------------
// set complete response frame
//-----------------------------------------------------------------------------
void setUartCompleteResponseFrame(
    uint8_t* pReplyBuffer,              // Pointer to the reply buffer
    uint8_t commandStatus,              // Status of the command
    uint16_t commandId,                 // Command ID
    uint16_t payloadLen,                // Payload length
    uint32_t* pNumReplyBytesWritten);   // Pointer to the number of reply bytes written

//-----------------------------------------------------------------------------
// an auxiliary function to Log array content in hex
//-----------------------------------------------------------------------------
void printHexAsLogMessage(
    const uint8_t* data,    // Data array
    uint32_t length);       //Length of the data

#endif