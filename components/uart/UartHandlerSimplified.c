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


/* PCNT unit */
extern pcnt_unit_handle_t pcnt_unit;

/* PCNT threshold value */
extern int pcntThreshold;

/* A queue to handle Uart radar trigger events */
QueueHandle_t uart_evt_queue;

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
    /* Set the log level */
    static const char *TAG = "UART_HANDLE_BUFFER_SIMPLIFIED";
    esp_log_level_set(TAG, ESP_LOG_INFO);

    //-----------------------------------------------------------------------------
    // by default, nothing is written to the reply buffer
    //-----------------------------------------------------------------------------
    *pNumReplyBytesWritten = 0;

    //-----------------------------------------------------------------------------
    // post packet should at least be the command size
    //-----------------------------------------------------------------------------
    if (postSizeInBytes < SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE)
    {
        ESP_LOGI(TAG, "Received packet is too small (received:%lu < expected:%d)", postSizeInBytes, SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
        return;
    }

    //-----------------------------------------------------------------------------
    // reply size should at least be the command size
    //-----------------------------------------------------------------------------
    if (replySizeInBytes < SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE)
    {
        ESP_LOGI(TAG, "Reply size is too small (received:%lu < expected:%d)", replySizeInBytes, SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
        return;
    }

    //-----------------------------------------------------------------------------
    // check the start symbol
    //-----------------------------------------------------------------------------
    if (pPostBuffer[0] != startSymbol)
    {
        ESP_LOGI(TAG, "Invalid start symbol is received");
        return;
    }

    //-----------------------------------------------------------------------------
    // check the stop symbol
    //-----------------------------------------------------------------------------
    if (pPostBuffer[postSizeInBytes-1] != stopSymbol)
    {
        ESP_LOGI(TAG, "Invalid stop symbol is received");
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
        ESP_LOGI(TAG, "Radar trigger command is received");
        handleRadarTriggerCommand();
    }
    else if (memcmp(pCommand, setDesiredNumberOfTriggerCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        ESP_LOGI(TAG, "Set desired number of Radar trigger command is received");
        handleSetDesiredNumberOfTriggerCommand(pCommand, postSizeInBytes);
    }
    else if ((memcmp(pCommand, clearNumberOfTriggerCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
        && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        ESP_LOGI(TAG, "Clear number of Radar trigger command is received");
        handleClearNumberOfTriggerCommand();
    }

    else if (memcmp(pCommand, setPulseCountCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        ESP_LOGI(TAG, "Set pulse count command is received");
        handleSetPulseCountCommand(pCommand, postSizeInBytes);
    }
    else if ((memcmp(pCommand, resetPcntCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
            && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        ESP_LOGI(TAG, "Reset command is received");
        handleResetPcntCommand();
    }
    else if ((memcmp(pCommand, pausePcntCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
            && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        ESP_LOGI(TAG, "Pause command is received");
        handlePausePcntCommand();
    }
    else if ((memcmp(pCommand, resumePcntCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
            && (postSizeInBytes == SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE))
    {
        ESP_LOGI(TAG, "Resume command is received");
        handleResumePcntCommand();
    }

    else if (memcmp(pCommand, setNumMeasurementCommand, SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE) == 0)
    {
        ESP_LOGI(TAG, "Set number of measurement command is received");
        handleSetNumMeasurementCommand(pCommand, postSizeInBytes);
    }

    else
    {
        ESP_LOGI(TAG, "Invalid command is received");
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
    /* Set the log level */
    static const char *TAG = "UART_SET_NUM_TRIGGER_COMMAND";
    esp_log_level_set(TAG, ESP_LOG_INFO);
    
    // Set the last character to NULL
    char desiredTriggerParameter[5]; // Maximum 5 characters
        
    if((postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE) <=5)
    {
        strncpy(desiredTriggerParameter,
            (const char*)pCommand+SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE,
            postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
    }
    else {
        ESP_LOGI(TAG, "Configuration parameter is too long");
        return;
    }
        
    // Read the parameter
    int desiredTrigger;
    desiredTrigger = atoi(desiredTriggerParameter);
     
    if (desiredTrigger > 0 )
    {
        ESP_LOGI(TAG, "Current desired trigger value is: %d", desiredTrigger);
        
        uart_evt_t evt;
        evt.command = UART_DESIRED_NUM_TRIGGER_COMMAND;
        evt.data = (uint32_t)desiredTrigger;
        xQueueSend(uart_evt_queue, &evt, 0 / portTICK_PERIOD_MS);
    }
    else
    {
        ESP_LOGI(TAG, "There is no valid configuration parameter in the command");
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
    /* Set the log level */
    static const char *TAG = "UART_SET_PULSE_COUNT_COMMAND";
    esp_log_level_set(TAG, ESP_LOG_INFO);

    // Set the last character to NULL
    char pulseCountParameter[5]; // Maximum 5 characters for the int16_t
        
    if((postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE) <=5)
    {
        strncpy(pulseCountParameter,
            (const char*)pCommand+SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE,
            postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
    }
    else {
        ESP_LOGI(TAG, "Configuration parameter is too long");
        return;
    }
        
    // Read the parameter
    int pcntThresholdNew;
    pcntThresholdNew = atoi(pulseCountParameter);
     
    if (pcntThresholdNew > 0 )
    {
        /* stop and clear the counter */
        ESP_ERROR_CHECK(pcnt_unit_stop(pcnt_unit));
        ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));

        ESP_ERROR_CHECK(pcnt_unit_remove_watch_point(pcnt_unit, pcntThreshold));
        ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, pcntThresholdNew));
        ESP_LOGI(TAG, "Pulse counter threshold %d is changed to %d", pcntThreshold, pcntThresholdNew);
        pcntThreshold = pcntThresholdNew;

        /* start the counter */
        ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
    }
    else
    {
        ESP_LOGI(TAG, "There is no valid configuration parameter in the command");
    }
}

//-----------------------------------------------------------------------------
// handle the reset command
//-----------------------------------------------------------------------------
void handleResetPcntCommand(void)
{
    /* stop, clear and resume to counting */
    ESP_ERROR_CHECK(pcnt_unit_stop(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
}

//-----------------------------------------------------------------------------
// handle the pause command
//-----------------------------------------------------------------------------
void handlePausePcntCommand(void)
{
    /* stop the counting */
    ESP_ERROR_CHECK(pcnt_unit_stop(pcnt_unit));
}

//-----------------------------------------------------------------------------
// handle the resume command
//-----------------------------------------------------------------------------
void handleResumePcntCommand(void)
{
    /* resume to counting */
   ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
}

//-----------------------------------------------------------------------------
// handle the set number of measurements command
//-----------------------------------------------------------------------------
void handleSetNumMeasurementCommand(const uint8_t* pCommand,
                                    uint32_t postSizeInBytes)
{
    /* Set the log level */
    static const char *TAG = "UART_SET_NUM_MEAS_COMMAND";
    esp_log_level_set(TAG, ESP_LOG_INFO);

    // Set the last character to NULL
    char numMeasurementParameter[5]; // Maximum 5 characters for the int16_t
        
    if((postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE) <=5)
    {
        strncpy(numMeasurementParameter,
            (const char*)pCommand+SIMPLIFIED_UART_PROTOCOL_COMMAND_SIZE,
            postSizeInBytes-SIMPLIFIED_UART_PROTOCOL_MIN_PACKET_SIZE);
    }
    else {
        ESP_LOGI(TAG, "Configuration parameter is too long");
        return;
    }
        
    // Read the parameter
    uint16_t numMeasurement;
    numMeasurement = atoi(numMeasurementParameter);
     
    if (numMeasurement > 0 )
    {
        ESP_LOGI(TAG, "Number of measurement value is: %u", numMeasurement);
    }
    else
    {
        ESP_LOGI(TAG, "There is no valid configuration parameter in the command");
    }
}