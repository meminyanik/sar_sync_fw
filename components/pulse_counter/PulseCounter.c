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

	PulseCounter.c

  Abstract:

	The implementation file of the Pulse Counter handler
*/

#include <PulseCounter.h>


/* PCNT unit */
pcnt_unit_handle_t pcnt_unit;

/* A queue to handle pulse counter events */
QueueHandle_t pcnt_evt_queue;

/* PCNT threshold value */
int pcntThreshold;

/* PCNT's event callback
 * pass the event data to the main program using a queue.
 */
static bool pcnt_handler_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_wakeup;
    QueueHandle_t queue = (QueueHandle_t)user_ctx;
    
    /* clear the counter if threshold is reached */
    if (edata->watch_point_value == pcntThreshold) {
        ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    }

    /* send watch point to queue, from this interrupt callback */
    xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
    
    /* return whether a high priority task has been waken up by this function */
    return (high_task_wakeup == pdTRUE);
}


/* Initialize PCNT functions:
 *  - configure and initialize PCNT
 *  - set up the input filter
 *  - set up the counter events to watch
 */
void pcntInitialize(void)
{
    /* Set the log level */
    static const char *TAG = "PCNT_INIT";
    esp_log_level_set(TAG, ESP_LOG_INFO);   
    
    /* install pcnt unit */
    pcnt_unit_config_t unit_config = {
        .high_limit = PCNT_H_LIM_VAL,
        .low_limit = PCNT_L_LIM_VAL,
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit)); 
    
    /* set glitch filter, ignore pulses lasting shorter than this */
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 125,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    /* install pcnt channel */
    pcnt_chan_config_t chan_config = {
        .edge_gpio_num = PCNT_INPUT_EDGE_IO,
        .level_gpio_num = -1,
    };
    pcnt_channel_handle_t pcnt_chan = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config, &pcnt_chan)); 
   
    /* set edge action for pcnt channels: keep the counter on rising edge, increase the counter on falling edge */
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan, PCNT_CHANNEL_EDGE_ACTION_HOLD, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    
    /* add watch point */
    pcntThreshold = 10;
    ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, pcntThreshold));

    /* register callbacks */
    pcnt_event_callbacks_t cbs = {
        .on_reach = pcnt_handler_on_reach,
    };
    ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(pcnt_unit, &cbs, pcnt_evt_queue));
    
     /* Enable, clear, and start pcnt unit */
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
}