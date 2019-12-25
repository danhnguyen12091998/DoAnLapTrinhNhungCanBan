#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx_tim.h"

void timer_init(void);
void timer_isr (void);
void timer_delay (const uint32_t time);
#endif
