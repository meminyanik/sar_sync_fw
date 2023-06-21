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

#include "Config.h"
#include "driver/pulse_cnt.h"


#define PCNT_H_LIM_VAL      	10000
#define PCNT_L_LIM_VAL     		0
#define PCNT_INPUT_EDGE_IO 		0  // Pulse Input GPIO (Edge)


/* Initialize PCNT functions:
 *  - configure and initialize PCNT
 *  - set up the input filter
 *  - set up the counter events to watch
 */
void pcntInitialize(void);

#endif