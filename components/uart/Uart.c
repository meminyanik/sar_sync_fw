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
#include <UartHandlerSimplified.h>


// Create RX and TX buffers
static uint8_t sUartRxBuffer[UART_BUFFER_SIZE];
static char sUartTxBuffer[UART_BUFFER_SIZE];

// Define the UART number
#ifdef UART_DEBUG_MODE
    const int UART_HOST_PC = UART_NUM_2;
#else
    const int UART_HOST_PC = UART_NUM_0;
#endif

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
        const int rxBytes = uart_read_bytes(UART_HOST_PC, sUartRxBuffer, UART_BUFFER_SIZE, 10 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            // Handle the buffer content
            uartHandleBufferSimplified(sUartRxBuffer,
                        rxBytes,
                        (uint8_t*)sUartTxBuffer,
                        UART_BUFFER_SIZE,
                        &numReplyBytesWritten);
            
            // Send the TX buffer data
            if (numReplyBytesWritten > 0)
            {
                sendUartData(sUartTxBuffer, numReplyBytesWritten);
            }
        }
    }
}

void uartInitialize(void) {
    /* Set the log level */
    static const char *TAG = "UART_INIT";
    esp_log_level_set(TAG, ESP_LOG_INFO);

    // Set UART configuration parameters
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // Configure UART
    ESP_ERROR_CHECK(uart_driver_install(UART_HOST_PC, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_HOST_PC, &uart_config));
#ifdef UART_DEBUG_MODE
    ESP_ERROR_CHECK(uart_set_pin(UART_HOST_PC, UART_DATA_TXD_PIN, UART_DATA_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
#else
    ESP_ERROR_CHECK(uart_set_pin(UART_HOST_PC, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
#endif
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
        ESP_LOGI(TAG, "The UART Task could not created.");
    }
}
