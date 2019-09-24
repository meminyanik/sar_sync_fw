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

	RadarTrigger.c

  Abstract:

	The implementation file of the Radar Trigger handler
*/

#include <Uart.h>
#include <PulseCounter.h>
#include <RadarTrigger.h>


/* The Radar Trigger Task */
void radarTriggerTask(void* params)
{
    /* The parameter value is expected to be NULL. */
    configASSERT(params == NULL);

    pcnt_evt_t pcnt_evt;
    uart_evt_t uart_evt;
    portBASE_TYPE res;
    
    /* Create the radar trigger queue set */
    radar_trigger_queue_set = xQueueCreateSet(RADAR_TRIGGER_QUEUE_SET_LENGTH);

    /* Initialize PCNT and UART event queues */
    pcnt_evt_queue = xQueueCreate(PCNT_EVT_QUEUE_LENGTH, sizeof(pcnt_evt_t));
    uart_evt_queue = xQueueCreate(UART_EVT_QUEUE_LENGTH, sizeof(uart_evt_t));

    /* Check everything was created. */
    configASSERT(radar_trigger_queue_set);
    configASSERT(pcnt_evt_queue);
    configASSERT(uart_evt_queue);

    /* Add the queues to the set */
    xQueueAddToSet(pcnt_evt_queue, radar_trigger_queue_set);
    xQueueAddToSet(uart_evt_queue, radar_trigger_queue_set);

    /* Start Task Loop */
    while (1) {
        /* 
            Block to wait for something to be available from the queues
            Don't block longer than 1000 ms
        */
        radar_trigger_queue_activated = xQueueSelectFromSet(radar_trigger_queue_set,
                                                            1000 / portTICK_PERIOD_MS );

        /* Which set member was selected?  Receives/takes can use a block time
        of zero as they are guaranteed to pass because xQueueSelectFromSet()
        would not have returned the handle unless something was available. */
        if(radar_trigger_queue_activated == pcnt_evt_queue)
        {
            /* 
                Wait for the event information passed from PCNT's interrupt handler.
                Once received, decode the event type.
            */
            res = xQueueReceive(pcnt_evt_queue, &pcnt_evt, 0 / portTICK_PERIOD_MS);
            if (res == pdTRUE) {
                if (pcnt_evt.status & PCNT_STATUS_THRES0_M) {
                    triggerRadar();
                    pcnt_counter_clear(PCNT_UNIT);
                }
                if (pcnt_evt.status & PCNT_STATUS_L_LIM_M) {
                    printf("L_LIM EVT\n");
                }
                if (pcnt_evt.status & PCNT_STATUS_H_LIM_M) {
                    printf("H_LIM EVT\n");
                }
                if (pcnt_evt.status & PCNT_STATUS_ZERO_M) {
                    printf("ZERO EVT\n");
                }
            }
        }
        else if (radar_trigger_queue_activated == uart_evt_queue)
        {
            /* 
                Wait for the event information passed from UART task.
                Once received, decode the event status.
            */
            res = xQueueReceive(uart_evt_queue, &uart_evt, 0 / portTICK_PERIOD_MS);
            if (res == pdTRUE) {
                if (uart_evt == UART_RADAR_TRIGGER_COMMAND) {
                    triggerRadar();
                }
            }
        }

    }

    /* Free the ISR service handle */
    if(user_isr_handle) {
        esp_intr_free(user_isr_handle);
        user_isr_handle = NULL;
    }

    /* The task is created. */
    vTaskDelete(NULL);
}

// Initialize Radar Trigger
void radarTriggerInitialize(void)
{
    /* Prepare configuration for the Radar Trigger Gpio Pin */
    gpio_config_t radarTriggerGpioConfig = {
        //disable interrupt
        .intr_type = GPIO_PIN_INTR_DISABLE,
        //set as output mode
        .mode = GPIO_MODE_OUTPUT,
        //bit mask of the pins that you want to set,e.g.GPIO18/19
        .pin_bit_mask = GPIO_OUTPUT_PIN_SEL,
        //disable pull-down mode
        .pull_down_en = 0,
        //disable pull-up mode
        .pull_up_en = 0,
    };

    /* configure GPIO with the given settings */
    gpio_config(&radarTriggerGpioConfig);

    /* set the level of the pin to 0 */
    if (gpio_get_level(RADAR_TRIGGER_OUT_PIN) != 0)
    {
        gpio_set_level(RADAR_TRIGGER_OUT_PIN, 0);
    }

    /* Create the task, store the handle. */
    BaseType_t xReturned;
    xReturned = xTaskCreatePinnedToCore(
                        radarTriggerTask,      			/* Function that implements the task. */
                    	"RadarTriggerTask",     		/* Text name for the task. */
                    	DEFAULT_TASK_STACK_SIZE_BYTES,  /* Stack size in bytes. */
                    	NULL,               			/* Parameter passed into the task. */
                    	5,                      		/* Priority at which the task is created. */
                    	&xRadarTriggerTask,     		/* Used to pass out the created task's handle. */
                        0);	                    		/* Core number. */
    if( xReturned != pdPASS )
    {
        /* The task is not created. */
        printf("The Radar Trigger Task could not created.");
    }
}

/* Radar Trigger Command */
void triggerRadar(void)
{
    gpio_set_level(RADAR_TRIGGER_OUT_PIN, 1);
    gpio_set_level(RADAR_TRIGGER_OUT_PIN, 0);
}