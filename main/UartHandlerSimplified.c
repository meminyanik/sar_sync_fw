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

    Uart protocol handler (simplified version) implementation file

*/

#include <UartHandlerSimplified.h>
#include <string.h>
#include <RadarTrigger.h>


//-----------------------------------------------------------------------------
//  Define simplified protocol version variables
//-----------------------------------------------------------------------------
char simpleErrorResponse[] = "ERR";
char simpleSuccessResponse[] = "SUC";
char simpleRadarCommand[] = "RTG";

//-----------------------------------------------------------------------------
// handle the post buffer (simplified version)
// prepare the reply buffer (simplified version)
//-----------------------------------------------------------------------------
void uartHandleBufferSimplified(
    const uint8_t* pPostBuffer,
    uint32_t postSizeInBytes,
    uint8_t* pReplyBuffer,
    uint32_t replySizeInBytes,
    uint32_t* pNumReplyBytesWritten)
{
    //-----------------------------------------------------------------------------
    // post packet should at least be the command size
    //-----------------------------------------------------------------------------
    if (postSizeInBytes < SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE)
    {
        printf("Received packet is too small (received:%d < expected:%d).\n", postSizeInBytes, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE);
        memcpy(pReplyBuffer, simpleErrorResponse, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE);
        *pNumReplyBytesWritten = SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE;
        return;
    }

    //-----------------------------------------------------------------------------
    // reply size should at least be the command size
    //-----------------------------------------------------------------------------
    if (replySizeInBytes < SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE)
    {
        printf("Reply size is too small (received:%d < expected:%d).\n", replySizeInBytes, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE);
        *pNumReplyBytesWritten = 0;
        return;
    }

    //-----------------------------------------------------------------------------
    // handle the command
    //-----------------------------------------------------------------------------
    if (memcmp(pPostBuffer, simpleRadarCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        printf("Radar trigger command is received.\n");
        triggerRadar();
        memcpy(pReplyBuffer, simpleSuccessResponse, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE);
    }
    else
    {
        printf("Unknown command is received.\n");
        memcpy(pReplyBuffer, simpleErrorResponse, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE);
    }

    *pNumReplyBytesWritten = SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE;
}