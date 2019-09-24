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

	UartMessage.h

  Abstract:

	Uart protocol frame data structures header file
*/

#ifndef UART_MESSAGE_H
#define UART_MESSAGE_H

#include <stdio.h>


//-----------------------------------------------------------------------------
//  Uart Protocol 2-byte Preamble
//-----------------------------------------------------------------------------
#define UART_PROTOCOL_PREAMBLE 0x7570u  // "up (Uart Protocol)" in ASCII

//-----------------------------------------------------------------------------
//  Uart Protocol Version
//-----------------------------------------------------------------------------
#define UART_PROTOCOL_VERSION 0x01u

//-----------------------------------------------------------------------------
//  Uart Protocol Command status field
//-----------------------------------------------------------------------------
enum eUartCmdStatus
{
    UART_SUCCESS = 0,
    UART_INVALID_PREAMBLE,
    UART_PROTOCOL_VERSION_MISMATCH,
    UART_INVALID_CMD_STATUS,
    UART_UNKNOWN_CMD_ID,
    UART_INVALID_FRAME_LENGTH,
    UART_ERROR_IN_CHECKSUM,
    UART_INVALID_PAYLOAD,
    UART_OUTPUT_BUFFER_TOO_SMALL,
};

//-----------------------------------------------------------------------------
// Command ID field is set to zero in the response frame for protocol errors
// If there is an error in the protocol frame, it is indicated in the Command Status field
// there is no need to check Command ID field in a command/response architecture 
//-----------------------------------------------------------------------------
#define CMD_ID_ERROR_IN_PROTOCOL 0x0000u

//-----------------------------------------------------------------------------
// Define the command ids
//-----------------------------------------------------------------------------
enum eUartCmdId
{
    TRIGGER_RADAR = 0,
    SET_PULSE_COUNT,
    STOP_SYSTEM,
};

//-----------------------------------------------------------------------------
// Uart Protocol Header
//-----------------------------------------------------------------------------
typedef struct
{
    uint16_t preamble;              // start of packet, always UART_PROTOCOL_PREAMBLE (not included in checksum)
    uint8_t protocolVersion;        // protocol version, set to UART_PROTOCOL_VERSION
    uint8_t commandStatus;          // status of the command, defined in eUartCmdStatus
    uint16_t commandId;             // command ID, defined in eUartCmdId
    uint16_t payloadLen;            // length of the payload data, excluding header and checksum
}UartProtocolHeader;

//-----------------------------------------------------------------------------
// Uart Protocol Footer
//-----------------------------------------------------------------------------
typedef struct
{
    uint8_t checksum[2];            // not included in the checksum
}UartProtocolFooter;

//-----------------------------------------------------------------------------
// Uart Protocol Overhead Size
//-----------------------------------------------------------------------------
#define UART_PROTOCOL_OVERHEAD_SIZE_BYTES sizeof(UartProtocolHeader) + sizeof(UartProtocolFooter)

//-----------------------------------------------------------------------------
// The Uart message that will be passed to the command handlers
//-----------------------------------------------------------------------------
typedef struct
{
    const uint8_t* pPostPayload;    // Pointer to the post payload address
    uint16_t postPayloadLen;        // Post payload length
    uint8_t* pReplyPayload;         // Pointer to the reply payload address
    uint16_t replyPayloadLenMax;    // The available bytes for the payload length
    uint16_t* pReplyPayloadLen;     // Pointer to the reply payload length
}UartMessage;

#endif