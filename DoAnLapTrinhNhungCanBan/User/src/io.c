#include "io.h"

void io_init(void)
{
    //Preparations
    GPIO_InitTypeDef GPIO_InitStructure;

	/* enable GPIOD clock  */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	/* configure PD12, PD13, PD14 and PD15 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_GREEN | LED_RED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

void led_on(uint16_t led)
{
	GPIO_SetBits(LED_PORT, led);
}

void led_off(uint16_t led)
{
	GPIO_ResetBits(LED_PORT, led);
}

void led_toggle(uint16_t led)
{
	LED_PORT->ODR ^= led;
}

