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

    Uart protocol handler implementation file

*/

#include <UartHandler.h>
#include "esp_log.h"
#include <string.h>


//-----------------------------------------------------------------------------
// handle the post buffer
// prepare the reply buffer
//-----------------------------------------------------------------------------
void uartHandleBuffer(
    const uint8_t* pPostBuffer,
    uint32_t postSizeInBytes,
    uint8_t* pReplyBuffer,
    uint32_t replySizeInBytes,
    uint32_t* pNumReplyBytesWritten)
{
    /* Set the log level */
    static const char *TAG = "UART_HANDLE_BUFFER";
    esp_log_level_set(TAG, ESP_LOG_INFO);

    //-----------------------------------------------------------------------------
    // post packet should at least be the overhead size (i.e. header + footer)
    //-----------------------------------------------------------------------------
    if (postSizeInBytes < UART_PROTOCOL_OVERHEAD_SIZE_BYTES)
    {
        ESP_LOGI(TAG, "Received packet is too small (received:%lu < expected:%d)", postSizeInBytes, UART_PROTOCOL_OVERHEAD_SIZE_BYTES);
        setProtocolErrorResponseFrame(pReplyBuffer, UART_INVALID_FRAME_LENGTH, pNumReplyBytesWritten);
        return;
    }

    //-----------------------------------------------------------------------------
    // a Uart packet is broken up into 3 parts: header, data payload (variable size), and footer.
    //-----------------------------------------------------------------------------
    
    // fill the members of the header
    UartProtocolHeader uartHeader;
    getUartProtocolHeader(pPostBuffer, &uartHeader);
    
    //-----------------------------------------------------------------------------
    // check the preamble
    //-----------------------------------------------------------------------------
    if (uartHeader.preamble != UART_PROTOCOL_PREAMBLE)
    {
        ESP_LOGI(TAG, "Preamble is not correct (received:0x%.4X != expected:0x%.4X)", uartHeader.preamble, UART_PROTOCOL_PREAMBLE);
        setProtocolErrorResponseFrame(pReplyBuffer, UART_INVALID_PREAMBLE, pNumReplyBytesWritten);
        return;
    }

    //-----------------------------------------------------------------------------
    // check the length of the frame
    //-----------------------------------------------------------------------------
    uint32_t expectedFrameLength = uartHeader.payloadLen + UART_PROTOCOL_OVERHEAD_SIZE_BYTES;
    if (postSizeInBytes != expectedFrameLength)
    {
        ESP_LOGI(TAG, "Frame length is not correct (received:%lu != expected:%lu)", postSizeInBytes, expectedFrameLength);
        setProtocolErrorResponseFrame(pReplyBuffer, UART_INVALID_FRAME_LENGTH, pNumReplyBytesWritten);
        return;
    }

    //-----------------------------------------------------------------------------
    // validate the checksum
    //-----------------------------------------------------------------------------
    const uint8_t* pChecksum = pPostBuffer + sizeof(UartProtocolHeader) + uartHeader.payloadLen;
    uint8_t calculatedChecksum[2];
    calculateCheckSum(
        pPostBuffer + 2, // exclude the preamble
        uartHeader.payloadLen + (sizeof(UartProtocolHeader) - 2),
        calculatedChecksum);

    if ((pChecksum[0] != calculatedChecksum[0]) || (pChecksum[1] != calculatedChecksum[1]))
    {
        ESP_LOGI(TAG, "Checksum is not correct (received:0x%.2X%.2X != expected:0x%.2X%.2X)", pChecksum[0], pChecksum[1], calculatedChecksum[0], calculatedChecksum[1]);
        setProtocolErrorResponseFrame(pReplyBuffer, UART_ERROR_IN_CHECKSUM, pNumReplyBytesWritten);
        return;
    }

    //-----------------------------------------------------------------------------
    // check the protocol version
    //-----------------------------------------------------------------------------
    if (uartHeader.protocolVersion != UART_PROTOCOL_VERSION)
    {
        ESP_LOGI(TAG, "Protocol versions do not match (received:0x%.2X != expected:0x%.2X)", uartHeader.protocolVersion, UART_PROTOCOL_VERSION);
        setProtocolErrorResponseFrame(pReplyBuffer, UART_PROTOCOL_VERSION_MISMATCH, pNumReplyBytesWritten);
        return;
    }

    //-----------------------------------------------------------------------------
    // check the command status, it must be UART_SUCCESS for the command frames
    //-----------------------------------------------------------------------------
    if (uartHeader.commandStatus != UART_SUCCESS)
    {
        ESP_LOGI(TAG, "Command Status is not correct (received:0x%.2X != expected:0x%.2X)", uartHeader.commandStatus, UART_SUCCESS);
        setProtocolErrorResponseFrame(pReplyBuffer, UART_INVALID_CMD_STATUS, pNumReplyBytesWritten);
        return;
    }

    //-----------------------------------------------------------------------------
    // prepare the subsystem Uart message
    //-----------------------------------------------------------------------------
    // Set command status as UART_SUCCESS as default. Command handlers will handle it.
    uint8_t commandStatus = UART_SUCCESS;

    // Set reply payload length as 0 by default. Command handlers will handle it.
    uint16_t replyPayloadLen = 0;

    // Payload size has to fit in uint16_t, otherwise it will be a protocol error
    // uint16_t replyPayloadLenMax = replySizeInBytes - UART_PROTOCOL_OVERHEAD_SIZE_BYTES;

    // Prepare the message for the command handler
    // UartMessage uartMessage = {
    //     pPostBuffer + sizeof(UartProtocolHeader),
    //     uartHeader.payloadLen,
    //     pReplyBuffer + sizeof(UartProtocolHeader),
    //     replyPayloadLenMax,
    //     &replyPayloadLen,
    // };

    //-----------------------------------------------------------------------------
    // proces the uart message in the appropriate handler
    //-----------------------------------------------------------------------------
    switch (uartHeader.commandId)
    {
        case TRIGGER_RADAR:
        {
            ESP_LOGI(TAG, "Trigger radar command with ID: %d is received", TRIGGER_RADAR);
            break;
        }
        case SET_PULSE_COUNT:
        {
            ESP_LOGI(TAG, "Set pulse count command with ID: %d is received", SET_PULSE_COUNT);
            break;
        }
        case STOP_SYSTEM:
        {
            ESP_LOGI(TAG, "Stop system command with ID: %d is received", STOP_SYSTEM);
            break;
        }
        default:
        {
            ESP_LOGI(TAG, "Unknown Uart command with ID: %u is received", uartHeader.commandId);
            setProtocolErrorResponseFrame(pReplyBuffer, UART_UNKNOWN_CMD_ID, pNumReplyBytesWritten);
            return;
        }
    }

    setUartCompleteResponseFrame(pReplyBuffer, commandStatus, uartHeader.commandId, replyPayloadLen, pNumReplyBytesWritten);
}

//-----------------------------------------------------------------------------
// the checksum algorithm used is the 8-Bit Fletcher Algorithm (used in the TCP standard)
//-----------------------------------------------------------------------------
void calculateCheckSum(
    const uint8_t* pDataIn,     // input byte array
    uint32_t dataLength,        // length of the input byte array
    uint8_t* pChecksum)         // pointer to the checksum
{
    uint8_t firstByte = 0;
    uint8_t secondByte = 0;

    for (uint32_t i = 0; i < dataLength; i++)
    {
        firstByte = firstByte + pDataIn[i];
        secondByte = secondByte + firstByte;
    }

    pChecksum[0] = (firstByte & 0xFF);
    pChecksum[1] = (secondByte & 0xFF);
}

//-----------------------------------------------------------------------------
// set the header and write into the buffer
//-----------------------------------------------------------------------------
void setUartProtocolHeader(
    uint8_t* pReplyBuffer,
    uint8_t commandStatus,
    uint16_t commandId,
    uint16_t payloadLen)
{
    // set the header
    UartProtocolHeader uartProtocolHeader;

    uartProtocolHeader.preamble = UART_PROTOCOL_PREAMBLE;
    uartProtocolHeader.protocolVersion = UART_PROTOCOL_VERSION;
    uartProtocolHeader.commandStatus = commandStatus;
    uartProtocolHeader.commandId = commandId;
    uartProtocolHeader.payloadLen = payloadLen;

    // fill the buffer with the header
    memcpy(pReplyBuffer, &uartProtocolHeader, sizeof(UartProtocolHeader));
}

//-----------------------------------------------------------------------------
// get the header from the buffer
//-----------------------------------------------------------------------------
void getUartProtocolHeader(
    const uint8_t* pPostBuffer,
    UartProtocolHeader* pUartHeader)
{
    // fill the header from the buffer
    memcpy(pUartHeader, pPostBuffer, sizeof(UartProtocolHeader));
}

//-----------------------------------------------------------------------------
// set the footer and write into the buffer
//-----------------------------------------------------------------------------
void setUartProtocolFooter(
    uint8_t* pReplyBuffer,
    uint16_t payloadLen)
{
    // calculate the checksum and fill the buffer
    calculateCheckSum(
        pReplyBuffer + 2,    // exclude the preamble
        payloadLen + (sizeof(UartProtocolHeader) - 2),
        pReplyBuffer + payloadLen + sizeof(UartProtocolHeader));
}

//-----------------------------------------------------------------------------
// set the protocol error response frame
//-----------------------------------------------------------------------------
void setProtocolErrorResponseFrame(
    uint8_t* pReplyBuffer,
    uint8_t commandStatus,
    uint32_t* pNumReplyBytesWritten)
{
    uint16_t payloadLen = 0; // length defined in the protocol specification

    // create the header and write into the buffer
    setUartProtocolHeader(
        pReplyBuffer,
        commandStatus,
        CMD_ID_ERROR_IN_PROTOCOL,
        payloadLen);

    // create the footer and write into the buffer
    setUartProtocolFooter(
        pReplyBuffer,
        payloadLen);
    
    *pNumReplyBytesWritten = payloadLen + UART_PROTOCOL_OVERHEAD_SIZE_BYTES;
}

//-----------------------------------------------------------------------------
// set Uart complete response frame
//-----------------------------------------------------------------------------
void setUartCompleteResponseFrame(
    uint8_t* pReplyBuffer,
    uint8_t commandStatus,
    uint16_t commandId,
    uint16_t payloadLen,
    uint32_t* pNumReplyBytesWritten)
{
    // create the header and write into the buffer
    setUartProtocolHeader(
        pReplyBuffer,
        commandStatus,
        commandId,
        payloadLen);

    // data is already filled by the subsystem

    // create the footer and write into the buffer
    setUartProtocolFooter(
        pReplyBuffer,
        payloadLen);

    *pNumReplyBytesWritten = payloadLen + UART_PROTOCOL_OVERHEAD_SIZE_BYTES;
}

//-----------------------------------------------------------------------------
// an auxiliary function to Log array content in hex
//-----------------------------------------------------------------------------
void printHexAsLogMessage(
    const uint8_t* data,
    uint32_t length)
{
    for (uint32_t i = 0; i < length; ++i)
    {
        printf("0x%.2X ", data[i]);
    }
    printf("\n");
}