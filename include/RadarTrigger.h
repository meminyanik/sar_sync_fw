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

#ifndef RADAR_TRIGGER_H
#define RADAR_TRIGGER_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "esp_attr.h"
#include "esp_log.h"


#define PCNT_TEST_UNIT      PCNT_UNIT_0
#define PCNT_H_LIM_VAL      10
#define PCNT_L_LIM_VAL     -10
#define PCNT_THRESH1_VAL    5
#define PCNT_THRESH0_VAL   -5
#define PCNT_INPUT_SIG_IO   4  // Pulse Input GPIO
#define PCNT_INPUT_CTRL_IO  5  // Control GPIO HIGH=count up, LOW=count down
#define LEDC_OUTPUT_IO      18 // Output GPIO of a sample 1 Hz pulse generator


xQueueHandle pcntEvtQueue;   // A queue to handle pulse counter events
pcnt_isr_handle_t radarIsrHandle = NULL; //user's ISR service handle

/* A sample structure to pass events from the PCNT
 * interrupt handler to the main program.
 */
typedef struct {
    int unit;  // the PCNT unit that originated an interrupt
    uint32_t status; // information on the event type that caused the interrupt
} pcnt_evt_t;

void vRadarTriggerTask(void*);

#endif