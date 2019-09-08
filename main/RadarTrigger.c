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
#include <LedControl.h>
#include <RadarTrigger.h>


/* The Radar Trigger Task */
void radarTriggerTask(void* params)
{
    /* The parameter value is expected to be NULL. */
    configASSERT(params == NULL);

    /* Initialize LEDC to generate sample pulse signal */
    ledcInitialize();

    /* Initialize PCNT event queue and PCNT functions */
    pcnt_evt_queue = xQueueCreate(10, sizeof(pcnt_evt_t));
    pcntInitialize();

    int16_t count = 0;
    pcnt_evt_t evt;
    portBASE_TYPE res;

    while (1) {
        /* Wait for the event information passed from PCNT's interrupt handler.
         * Once received, decode the event type and print it on the serial monitor.
         */
        res = xQueueReceive(pcnt_evt_queue, &evt, 1000 / portTICK_PERIOD_MS);
        if (res == pdTRUE) {
            pcnt_get_counter_value(PCNT_TEST_UNIT, &count);
            printf("Event PCNT unit[%d]; cnt: %d\n", evt.unit, count);
            if (evt.status & PCNT_STATUS_THRES1_M) {
                printf("THRES1 EVT\n");
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
        } else {
            pcnt_get_counter_value(PCNT_TEST_UNIT, &count);
            printf("Current counter value :%d\n", count);
        }
    }

    /* Free the ISR service handle */
    // if(user_isr_handle) {
    //     esp_intr_free(user_isr_handle);
    //     user_isr_handle = NULL;
    // }

    /* The task is created. */
    vTaskDelete(NULL);
}