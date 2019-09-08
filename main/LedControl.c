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

	LedControl.c

  Abstract:

	The implementation file of the Led Control handler
*/

#include <LedControl.h>


/* Configure LED PWM Controller
 * to output sample pulses at 1 Hz with duty of about 10%
 */
void ledcInitialize(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer;
    ledc_timer.speed_mode       = LEDC_HIGH_SPEED_MODE;
    ledc_timer.timer_num        = LEDC_TIMER_1;
    ledc_timer.duty_resolution  = LEDC_TIMER_10_BIT;
    ledc_timer.freq_hz          = 1;  // set output frequency at 1 Hz
    ledc_timer_config(&ledc_timer);

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.channel    = LEDC_CHANNEL_1;
    ledc_channel.timer_sel  = LEDC_TIMER_1;
    ledc_channel.intr_type  = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num   = LEDC_OUTPUT_IO;
    ledc_channel.duty       = 100; // set duty at about 10%
    ledc_channel.hpoint     = 0;
    ledc_channel_config(&ledc_channel);
}