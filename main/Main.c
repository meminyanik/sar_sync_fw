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

	main.c

  Abstract:

	The entry point of the application
*/

#include "Config.h"
#include "Uart.h"
#include "RadarTrigger.h"
#include "PulseCounter.h"

#ifdef INTERNAL_TEST_MODE
    #include "LedControl.h"
#endif



/*
	This code counts pulses generated by the Motion Controller
	and triggers Radar after a predefined number of pulses

	Functionality of GPIOs used in this example:
     GPIO0 - pulse input pin,
     GPIO4 - radar trigger output pin.

	 GPIO2 - LEDC output for internal test
    
	To use this code, you should connect the pulse output of the Motion Controller to GPIO4.
  
	The radar is triggered when the counter value reaches a threshold value.
*/


void app_main(void)
{
	//-----------------------------------------------------
	// Initialize LEDC to generate sample pulse signal
	//-----------------------------------------------------
    #ifdef INTERNAL_TEST_MODE
		ledcInitialize();
	#endif

	//-----------------------------------------------------
	// Initialize Radar Trigger to generate trigger signal
	//-----------------------------------------------------
    radarTriggerInitialize();

	//-----------------------------------------------------
	// Initialize Pulse Counter to count pulses
	//-----------------------------------------------------
	pcntInitialize();

	//-----------------------------------------------------
	// Initialize Uart interface
	//-----------------------------------------------------
    uartInitialize();
	
}