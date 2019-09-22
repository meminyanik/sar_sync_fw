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


#include <string.h>
#include <stdlib.h>

#include <UartHandlerSimplified.h>
#include <Uart.h>
#include <PulseCounter.h>


//-----------------------------------------------------------------------------
//  Define simplified protocol version variables
//-----------------------------------------------------------------------------
static char radarTriggerCommand[] = "RTG";
static char setPulseCountCommand[] = "PLS";
static char setNumMeasurementCommand[] = "MSR";

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
    // by default, nothing is written to the reply buffer
    //-----------------------------------------------------------------------------
    *pNumReplyBytesWritten = 0;

    //-----------------------------------------------------------------------------
    // post packet should at least be the command size
    //-----------------------------------------------------------------------------
    if (postSizeInBytes < SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE)
    {
        printf("Received packet is too small (received:%d < expected:%d)\n", postSizeInBytes, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE);
        return;
    }

    //-----------------------------------------------------------------------------
    // reply size should at least be the command size
    //-----------------------------------------------------------------------------
    if (replySizeInBytes < SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE)
    {
        printf("Reply size is too small (received:%d < expected:%d)\n", replySizeInBytes, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE);
        return;
    }

    //-----------------------------------------------------------------------------
    // handle the command
    //-----------------------------------------------------------------------------
    if (memcmp(pPostBuffer, radarTriggerCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        printf("Radar trigger command is received\n");
        handleRadarTriggerCommand();
    }
    else if (memcmp(pPostBuffer, setPulseCountCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        printf("Set pulse count command is received\n");
        handleSetPulseCountCommand(pPostBuffer, postSizeInBytes);
    }
    else if (memcmp(pPostBuffer, setNumMeasurementCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        printf("Set number of measurement command is received\n");
        // handleSetNumMeasurementCommand();
    }
    else
    {
        printf("Unknown command is received\n");
    }
}

//-----------------------------------------------------------------------------
// handle the radar trigger command
//-----------------------------------------------------------------------------
void handleRadarTriggerCommand(void)
{
    uart_evt_t evt = UART_RADAR_TRIGGER_COMMAND;
    xQueueSend(uart_evt_queue, &evt, 0 / portTICK_PERIOD_MS);
}

//-----------------------------------------------------------------------------
// handle the radar trigger command
//-----------------------------------------------------------------------------
void handleSetPulseCountCommand(const uint8_t* pPostBuffer,
                                uint32_t postSizeInBytes)
{
    // Set the last character to NULL
    char pulseCountParameter[5]; // Maximum 5 characters for the int16_t
        
    if((postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) <=5)
    {
        memcpy(pulseCountParameter,
            pPostBuffer+SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE,
            postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE);
    }
    else {
        printf("Configuration parameter is too long\n");
        return;
    }
        
    // Read the parameter
    int pcntThreshold;
    pcntThreshold = atoi(pulseCountParameter);
     
    if (pcntThreshold > 0 )
    {
        printf("Current pulse counter value is: %d\n", pcntThreshold);
        pcnt_set_event_value(PCNT_UNIT, PCNT_EVT_THRES_0, pcntThreshold);
    }
    else
    {
        printf("There is no valid configuration parameter in the command\n");
    }
}

//-----------------------------------------------------------------------------
// handle the radar trigger command
//-----------------------------------------------------------------------------
void handleSetNumMeasurementCommand(uint16_t* pNumMeasurement)
{

}