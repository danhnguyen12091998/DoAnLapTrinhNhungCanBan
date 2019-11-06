#include "stm32f4xx.h"                  // Device header
#include "stm32f429i_discovery_lcd.h"
#include <stdio.h>
#include <string.h>

#define STR_MAX_LENGTH 100

#define TC_BIT 0x0040
#define TC_SHIFT_LEFT 6

#define RXNE_BIT 0x0020
#define RXNE_SHIFT_LEFT 5

void ldc_my_init(void);
void uart_my_init(void);
void uart_my_send_str_polling(const char* str);

int main()
{
	return 0;
}