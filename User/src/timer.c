#include "timer.h"

volatile uint32_t g_delay_time = 0;

void timer_init()
{
    // Init Timer
    // Clock = 16MHz
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	uint16_t prescaler_value;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable TIM7 peripheral clock  */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	/* To get TIM7 counter clock at 12 MHz, the prescaler is computed as follows:
	prescaler_value = (TIM7 clock / TIM2 counter clock) - 1
					= ((SystemCoreClock / 2) / 12 MHz) - 1 */
	prescaler_value = (uint16_t) ((SystemCoreClock / 2) / 12000000) - 1;

	TIM_TimeBaseStructure.TIM_Prescaler = prescaler_value;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_Period = 11999;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	/* Enable the TIM7 global interrupt. */
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable TIM7 interrupt source. */
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
}
/************************************************************************/
/**
  * @brief	Handler for Timer7
  * @param  None
  * @retval None
  */
void timer_isr (void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		if (g_delay_time > 0x00)
		{
			g_delay_time--;
		}
		else
		{
			TIM_Cmd(TIM7, DISABLE);
		}
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}
/************************************************************************/
/**
  * @brief		Delay routine based on Timer7.
  * @param[in]	time: delay time in ms.
  * @retval		None.
  */
void timer_delay (const uint32_t time)
{
	g_delay_time = time;
	
	/* Enable TIM7 counter. */
	TIM_Cmd(TIM7, ENABLE);

	while (g_delay_time != 0);
}


