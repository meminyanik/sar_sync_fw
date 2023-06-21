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
#define LEDC_OUTPUT_IO      2 

/* 
	Configure LED PWM Controller
 	to output sample pulses at 100 KHz with duty of about 25%
*/
void ledcInitialize(void);

#endif