/*
 * Date: 25/4/2016
 * Writer: ESS
 * Description: This file contains UART function implementations
 */

#include "uart.h"
#include "stm32f4xx_usart.h"

void uart3_init_interrupt(void)
{
    // Preparations
    GPIO_InitTypeDef gpio_init;
    USART_InitTypeDef usart_init;
    NVIC_InitTypeDef NVIC_InitStruct;

    // Initialize
    GPIO_StructInit(&gpio_init);
    USART_StructInit(&usart_init);

    // Enable peripheral clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    // According to the USART mode, enable the GPIO clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // Connect the pin to the desired peripherals' Alternate
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    // Configure the desired pin in alternate function
    // Select the type, pull-up/pull-down and output speed
    // Call GPIO_Init() function
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
    gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &gpio_init);

    // Program the Baud Rate, Word Length , Stop Bit, Parity, Hardware
    // flow control and Mode(Receiver/Transmitter) using the USART_Init()
    // function.
    usart_init.USART_BaudRate = 9600;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    usart_init.USART_Parity = USART_Parity_No;
    usart_init.USART_StopBits = USART_StopBits_1;
    usart_init.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART3, &usart_init);

    // Enable the NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_Init(&NVIC_InitStruct);

    // Enable USART3
    USART_Cmd(USART3, ENABLE);
}


