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

#include <PulseCounter.h>
#include <RadarTrigger.h>


/* The Radar Trigger Task */
void radarTriggerTask(void* params)
{
    /* The parameter value is expected to be NULL. */
    configASSERT(params == NULL);

    //int16_t count = 0;
    pcnt_evt_t evt;
    portBASE_TYPE res;
    
    /* Initialize PCNT event queue and PCNT functions */
    pcnt_evt_queue = xQueueCreate(10, sizeof(pcnt_evt_t));
    pcntInitialize();

    /* Start Task Loop */
    while (1) {
        /* 
            Wait for the event information passed from PCNT's interrupt handler.
            Once received, decode the event type and print it on the serial monitor.
         */
        res = xQueueReceive(pcnt_evt_queue, &evt, 1000 / portTICK_PERIOD_MS);
        if (res == pdTRUE) {
            //pcnt_get_counter_value(PCNT_UNIT, &count);
            //printf("Event PCNT unit[%d]; cnt: %d\n", evt.unit, count);
            if (evt.status & PCNT_STATUS_THRES0_M) {
                //printf("THRES0 EVT\n");
                pcnt_counter_clear(PCNT_UNIT);
                triggerRadar();
            }
            if (evt.status & PCNT_STATUS_L_LIM_M) {
                printf("L_LIM EVT\n");
            }
            if (evt.status & PCNT_STATUS_H_LIM_M) {
                printf("H_LIM EVT\n");
            }
            if (evt.status & PCNT_STATUS_ZERO_M) {
                printf("ZERO EVT\n");
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

    //configure GPIO with the given settings
    gpio_config(&radarTriggerGpioConfig);

    /* Create the task, storing the handle. */
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