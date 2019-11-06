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
static void uart_my_send_str_polling(const char* str);
void uart_my_receive_str_polling(char* str, uint8_t strLen);

static void uart_my_send_str_polling(const char* str)
{
	if(str != NULL)
	{
		uint8_t strIndex = 0;
		uint8_t strLen = strlen(str);
		for(strIndex = 0; strIndex < strLen; strIndex++)
		{
			// Send data
			USART_SendData(USART3, (uint16_t)str[strIndex]);
			
			// Waits until UART sends data completely
			while(((USART3->SR & TC_BIT) >> TC_SHIFT_LEFT) == 0)
			{
				// Do nothing
			}
		}	
	}
}

int main()
{
	// Preparations
	char* inputCharMsg = "Input your character: ";
	char* dontSupportStr = "Don't support";
	char* visibleCharStr = "Visible char";
	char* invisibleCharStr = "Invisible char";
	char* newLineStr = "\r\n";
	
	char charInput;
	char charInputStr[STR_MAX_LENGTH];
	memset(charInputStr, 0, STR_MAX_LENGTH);
	
	LCD_Init();
	
	uart_my_init();
	
	for(; ; )
	{
		uart_my_send_str_polling(inputCharMsg);
		
		uart_my_receive_str_polling(charInputStr, 1);
		
		LCD_ClearLine(0);
		
		charInput = charInputStr[0];
		
		if((charInput >= 32) && (charInput <= 126))
		{
			//
			LCD_DisplayStringLine(0, (uint8_t*)visibleCharStr);
			
			//
			uart_my_send_str_polling(charInputStr);
			uart_my_send_str_polling(newLineStr);			
		}		
		else
		{
			LCD_DisplayStringLine(0, (uint8_t*)invisibleCharStr);
			
			//
			uart_my_send_str_polling(dontSupportStr);
			uart_my_send_str_polling(newLineStr);			
		}
	}	
}

void lcd_my_init()
{
	LCD_Init();
	
	LCD_LayerInit();

	LTDC_Cmd(ENABLE);
	
	LCD_SetLayer(LCD_FOREGROUND_LAYER);		
}

void uart_my_init()
{
	
	//Preparations
	GPIO_InitTypeDef gpio_init;
	USART_InitTypeDef usart_init;
	
	// Initialize
	GPIO_StructInit(&gpio_init);
	USART_StructInit(&usart_init);
	
	// Enable peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// According to the USART mode, enable the GPIO clocks
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// Connect the pin to the desired peripheral's Alternate
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	// Configure the desired pin in alternate function
	// Select the type, pull-up/pull-down and output speed
	// Call GPIO_Init() function
	
	usart_init.USART_BaudRate = 9600;
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart_init.USART_Parity =  USART_Parity_No;
	usart_init.USART_StopBits = USART_StopBits_1;
	usart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &usart_init);
	
	// Enable USART3
	USART_Cmd(USART3, ENABLE);
	
}

void uart_my_receive_str_polling(char* str, uint8_t strLen)
{
	if(str != NULL)
	{
		uint8_t strIndex = 0;
		for(strIndex = 0; strIndex < strLen; strIndex++)
		{
			// Waits until UART receive data completely
			while(((USART3->SR & RXNE_BIT) >> RXNE_SHIFT_LEFT) == 0)
			{
				// Do nothing
			}
			
			str[strIndex] = (char) USART_ReceiveData(USART3);
		}
	}	
}

