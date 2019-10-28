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
static char startSymbol = '$';
static char stopSymbol = '#';

static char radarTriggerCommand[] = "RTG";
static char setDesiredNumberOfTriggerCommand[] = "DTG";
static char clearNumberOfTriggerCommand[] = "CTG";

static char setPulseCountCommand[] = "PLS";
static char resetPcntCommand[] = "RST";
static char pausePcntCommand[] = "PAU";
static char resumePcntCommand[] = "RES";

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
    if (postSizeInBytes < SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE)
    {
        printf("Received packet is too small (received:%d < expected:%d)\n", postSizeInBytes, SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
        return;
    }

    //-----------------------------------------------------------------------------
    // reply size should at least be the command size
    //-----------------------------------------------------------------------------
    if (replySizeInBytes < SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE)
    {
        printf("Reply size is too small (received:%d < expected:%d)\n", replySizeInBytes, SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
        return;
    }

    //-----------------------------------------------------------------------------
    // check the start symbol
    //-----------------------------------------------------------------------------
    if (pPostBuffer[0] != startSymbol)
    {
        printf("Invalid start symbol is received\n");
        return;
    }

    //-----------------------------------------------------------------------------
    // check the stop symbol
    //-----------------------------------------------------------------------------
    if (pPostBuffer[postSizeInBytes-1] != stopSymbol)
    {
        printf("Invalid stop symbol is received\n");
        return;
    }

    //-----------------------------------------------------------------------------
    // get the pointer for the command part
    //-----------------------------------------------------------------------------
    const uint8_t* pCommand = pPostBuffer + 1;

    //-----------------------------------------------------------------------------
    // handle the command
    //-----------------------------------------------------------------------------
    if ((memcmp(pCommand, radarTriggerCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
        && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        printf("Radar trigger command is received\n");
        handleRadarTriggerCommand();
    }
    else if (memcmp(pCommand, setDesiredNumberOfTriggerCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        printf("Set desired number of Radar trigger command is received\n");
        handleSetDesiredNumberOfTriggerCommand(pCommand, postSizeInBytes);
    }
    else if ((memcmp(pCommand, clearNumberOfTriggerCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
        && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        printf("Clear number of Radar trigger command is received\n");
        handleClearNumberOfTriggerCommand();
    }

    else if (memcmp(pCommand, setPulseCountCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        printf("Set pulse count command is received\n");
        handleSetPulseCountCommand(pCommand, postSizeInBytes);
    }
    else if ((memcmp(pCommand, resetPcntCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
            && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        printf("Reset command is received\n");
        handleResetPcntCommand();
    }
    else if ((memcmp(pCommand, pausePcntCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
            && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        printf("Pause command is received\n");
        handlePausePcntCommand();
    }
    else if ((memcmp(pCommand, resumePcntCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
            && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        printf("Resume command is received\n");
        handleResumePcntCommand();
    }

    else if (memcmp(pCommand, setNumMeasurementCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        printf("Set number of measurement command is received\n");
        handleSetNumMeasurementCommand(pCommand, postSizeInBytes);
    }

    else
    {
        printf("Invalid command is received\n");
    }
}

//-----------------------------------------------------------------------------
// handle the radar trigger command
//-----------------------------------------------------------------------------
void handleRadarTriggerCommand(void)
{
    uart_evt_t evt;
    evt.command = UART_RADAR_TRIGGER_COMMAND;
    xQueueSend(uart_evt_queue, &evt, 0 / portTICK_PERIOD_MS);
}

//-----------------------------------------------------------------------------
// handle the set desired number of radar trigger command
//-----------------------------------------------------------------------------
void handleSetDesiredNumberOfTriggerCommand(const uint8_t* pCommand,
                                        uint32_t postSizeInBytes)
{
    // Set the last character to NULL
    char desiredTriggerParameter[5]; // Maximum 5 characters
        
    if((postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE) <=5)
    {
        strncpy(desiredTriggerParameter,
            (const char*)pCommand+SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE,
            postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
    }
    else {
        printf("Configuration parameter is too long\n");
        return;
    }
        
    // Read the parameter
    int desiredTrigger;
    desiredTrigger = atoi(desiredTriggerParameter);
     
    if (desiredTrigger > 0 )
    {
        printf("Current desired trigger value is: %d\n", desiredTrigger);
        
        uart_evt_t evt;
        evt.command = UART_DESIRED_NUM_TRIGGER_COMMAND;
        evt.data = (uint32_t)desiredTrigger;
        xQueueSend(uart_evt_queue, &evt, 0 / portTICK_PERIOD_MS);
    }
    else
    {
        printf("There is no valid configuration parameter in the command\n");
    }
}

//-----------------------------------------------------------------------------
// handle the clear number of radar trigger command
//-----------------------------------------------------------------------------
void handleClearNumberOfTriggerCommand(void)
{
    uart_evt_t evt;
    evt.command = UART_CLEAR_NUM_TRIGGER_COMMAND;
    xQueueSend(uart_evt_queue, &evt, 0 / portTICK_PERIOD_MS);
}

//-----------------------------------------------------------------------------
// handle the set pulse count command
//-----------------------------------------------------------------------------
void handleSetPulseCountCommand(const uint8_t* pCommand,
                                uint32_t postSizeInBytes)
{
    // Set the last character to NULL
    char pulseCountParameter[5]; // Maximum 5 characters for the int16_t
        
    if((postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE) <=5)
    {
        strncpy(pulseCountParameter,
            (const char*)pCommand+SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE,
            postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
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
// handle the reset command
//-----------------------------------------------------------------------------
void handleResetPcntCommand(void)
{
    /* pause, clear and resume to counting */
    pcnt_counter_pause(PCNT_UNIT);
    pcnt_counter_clear(PCNT_UNIT);
    pcnt_counter_resume(PCNT_UNIT);
}

//-----------------------------------------------------------------------------
// handle the pause command
//-----------------------------------------------------------------------------
void handlePausePcntCommand(void)
{
    /* pause the counting */
    pcnt_counter_pause(PCNT_UNIT);
}

//-----------------------------------------------------------------------------
// handle the resume command
//-----------------------------------------------------------------------------
void handleResumePcntCommand(void)
{
    /* resume to counting */
    pcnt_counter_resume(PCNT_UNIT);
}

//-----------------------------------------------------------------------------
// handle the set number of measurements command
//-----------------------------------------------------------------------------
void handleSetNumMeasurementCommand(const uint8_t* pCommand,
                                    uint32_t postSizeInBytes)
{
    // Set the last character to NULL
    char numMeasurementParameter[5]; // Maximum 5 characters for the int16_t
        
    if((postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE) <=5)
    {
        strncpy(numMeasurementParameter,
            (const char*)pCommand+SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE,
            postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
    }
    else {
        printf("Configuration parameter is too long\n");
        return;
    }
        
    // Read the parameter
    uint16_t numMeasurement;
    numMeasurement = atoi(numMeasurementParameter);
     
    if (numMeasurement > 0 )
    {
        printf("Number of measurement value is: %d\n", numMeasurement);
    }
    else
    {
        printf("There is no valid configuration parameter in the command\n");
    }
}