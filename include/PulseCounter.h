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

	PulseCounter.h

  Abstract:

	The header file of the Pulse Counter handler
*/

#ifndef PULSE_COUNTER_H
#define PULSE_COUNTER_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/pcnt.h"
#include "esp_attr.h"


#define PCNT_UNIT      		PCNT_UNIT_0
#define PCNT_H_LIM_VAL      10000
#define PCNT_L_LIM_VAL     	0
#define PCNT_THRES_VAL  	5
#define PCNT_INPUT_SIG_IO   4  // Pulse Input GPIO
#define PCNT_INPUT_CTRL_IO  5  // Control GPIO HIGH=count up, LOW=count down


/* A queue to handle pulse counter events */
xQueueHandle pcnt_evt_queue;

/* user's ISR service handle */
pcnt_isr_handle_t user_isr_handle;

/* A sample structure to pass events from the PCNT
 * interrupt handler to the main program.
 */
typedef struct {
    int unit;  // the PCNT unit that originated an interrupt
    uint32_t status; // information on the event type that caused the interrupt
} pcnt_evt_t;

/* Initialize PCNT functions:
 *  - configure and initialize PCNT
 *  - set up the input filter
 *  - set up the counter events to watch
 */
void pcntInitialize(void);

#endif