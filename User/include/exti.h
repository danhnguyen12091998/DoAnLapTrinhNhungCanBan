/************************************************************************/
/*	History										Prepared/Updated by		*/
/*		+28/10/2013: initial version			cqkhanh					*/
/************************************************************************/

#ifndef __EXTI_H__
#define __EXTI_H__

#include "stm32f4xx.h"

#ifdef __cplusplus
 extern "C" {
#endif

/* extern function */
void exti_init(void);
void exti_isr(void);

#ifdef __cplusplus
}
#endif

#endif /* __EXTI_H__ */
/************************************************************************/
