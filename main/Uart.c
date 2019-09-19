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

	Uart.c

  Abstract:

	The implementation file of the Uart handler
*/

#include <Uart.h>
#include <UartHandler.h>

// Create RX and TX buffers
uint8_t sUartRxBuffer[UART_BUFFER_SIZE];
char sUartTxBuffer[UART_BUFFER_SIZE+2];

// Define the UART number
const int UART_HOST_PC = UART_NUM_0;

int sendUartData(const char* data, uint32_t length)
{
    const int txBytes = uart_write_bytes(UART_HOST_PC, data, length);
    return txBytes;
}

void uartTask(void *arg)
{
    // prepare the number of reply bytes
    uint32_t numReplyBytesWritten = 0;

    // Read the packet and process it
    while (1) {
        const int rxBytes = uart_read_bytes(UART_HOST_PC, sUartRxBuffer, UART_BUFFER_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0) {
            // Handle the buffer content
            uartHandleBufferSimple(sUartRxBuffer,
                            rxBytes,
                            (uint8_t*)sUartTxBuffer,
                            UART_BUFFER_SIZE,
                            &numReplyBytesWritten);

            // Send the TX buffer data
            sendUartData(sUartTxBuffer, numReplyBytesWritten);
        }
    }
}

void uartInitialize(void) {
    // Set UART configuration parameters
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // Configure UART
    ESP_ERROR_CHECK(uart_param_config(UART_HOST_PC, &uart_config));

    // Set UART pins
    ESP_ERROR_CHECK(uart_set_pin(UART_HOST_PC,
                                UART_PIN_NO_CHANGE,
                                UART_PIN_NO_CHANGE,
                                UART_PIN_NO_CHANGE,
                                UART_PIN_NO_CHANGE));
    
    // Install UART driver
    ESP_ERROR_CHECK(uart_driver_install(UART_HOST_PC,
                                        UART_BUFFER_SIZE,
                                        UART_BUFFER_SIZE,
                                        0,
                                        NULL,
                                        0));

    // Flush the UART (discard the existing data)
    ESP_ERROR_CHECK(uart_flush(UART_HOST_PC));

    // Create the UART task
    BaseType_t xReturned;
    xReturned = xTaskCreatePinnedToCore(
                    uartTask,
                    "UartTask",
                    DEFAULT_TASK_STACK_SIZE_BYTES,
                    NULL,
                    configMAX_PRIORITIES,
                    NULL,
                    1);
    if( xReturned != pdPASS )
    {
        /* The task is not created. */
        printf("The UART Task could not created.");
    }
}
