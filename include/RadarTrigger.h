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

	RadarTrigger.h

  Abstract:

	The header file of the Radar Trigger handler
*/

#ifndef RADAR_TRIGGER_H
#define RADAR_TRIGGER_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "esp_attr.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#include <Config.h>

// Output GPIO of the Radar Trigger
#define RADAR_TRIGGER_OUT_PIN      19
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<RADAR_TRIGGER_OUT_PIN)

/* 
	Radar Trigger task has two queues
	The first one is from the Pulse Counter for automated trigger
	The second one is for the manual trigers coming from the Uart interface
	Define the lengths of the queues that will be added to the queue set.
*/
#define PCNT_EVT_QUEUE_LENGTH		10
#define UART_EVT_QUEUE_LENGTH		10

#define RADAR_TRIGGER_QUEUE_SET_LENGTH ( PCNT_EVT_QUEUE_LENGTH + UART_EVT_QUEUE_LENGTH)

/* A queue set to handle radar trigger events */
QueueSetHandle_t radar_trigger_queue_set;
QueueSetMemberHandle_t radar_trigger_queue_activated;

/* A task handle for the radar trigger */
TaskHandle_t xRadarTriggerTask;

/* Initialize Radar Trigger */
void radarTriggerInitialize(void);

/* Radar Trigger Command */
void triggerRadar(void);

#endif