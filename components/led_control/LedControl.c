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
    ledc_timer_config_t ledc_timer = {
      .speed_mode       = LEDC_MODE,
      .timer_num        = LEDC_TIMER,
      .duty_resolution  = LEDC_DUTY_RES,
      .freq_hz          = LEDC_FREQUENCY, 
      .clk_cfg          = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
      .speed_mode = LEDC_MODE,
      .channel    = LEDC_CHANNEL,
      .timer_sel  = LEDC_TIMER,
      .intr_type  = LEDC_INTR_DISABLE,
      .gpio_num   = LEDC_OUTPUT_IO,
      .duty       = LEDC_DUTY,
      .hpoint     = 0,
    };
    // create the pulses immediately
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}