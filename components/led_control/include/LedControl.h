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

	LedControl.h

  Abstract:

	The header file of the Led Control handler
*/

#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "Config.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

// Output GPIO of a sample 100 KHz pulse generator
#define LEDC_TIMER          LEDC_TIMER_0
#define LEDC_CHANNEL        LEDC_CHANNEL_0
#define LEDC_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_DUTY_RES       LEDC_TIMER_8_BIT 	// Set duty resolution to 8 bits
#define LEDC_FREQUENCY      100000 				// Frequency in Hertz. Set frequency at 100 KHz (10us interval)
#define LEDC_DUTY           26 					// Set duty to 10% (1us pulses). ((2 ** 8) - 1) * 10% ~= 26		<-	(2**duty_resolution - 1) * duty_cycle

#define LEDC_OUTPUT_IO      2 

/* 
	Configure LED PWM Controller
 	to output sample pulses at 100 KHz with duty of about 25%
*/
void ledcInitialize(void);

#endif