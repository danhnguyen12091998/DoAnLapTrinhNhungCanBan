/*
 * Date: 28/3/2016
 * Module: lcd
 * Writer: ESS
 * Description: This file contains LCD function declarations
 */

// Include header files


#ifndef __IO_H__
#define __IO_H__

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


#define LED_PORT        GPIOG
#define LED_RED         GPIO_Pin_14
#define LED_GREEN       GPIO_Pin_13
// Function declarations

/* Name: io_init
 * Module: IO
 * Parameters: void
 * Return: void
 * Description: Initialize IO
 */
void io_init(void);

void led_on(uint16_t led);

void led_off(uint16_t led);

void led_toggle(uint16_t led);

#endif

/* End of file */
