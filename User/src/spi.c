

#include "spi.h"
#include "stm32f4xx_spi.h"

void spi_Init(void)
{
    // Preparations
    SPI_InitTypeDef spi_init;
    GPIO_InitTypeDef gpio_init;
    NVIC_InitTypeDef nvic_init;
    
    // Initialize
    SPI_StructInit(&spi_init);
    GPIO_StructInit(&gpio_init);
    
    // Enable peripheral clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    
    
    //SPI 1 Master
    // enable clock for used IO pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    /* Pin_A_5 = CLK
       Pin_A_6 = MISO
       Pin_A_7 = MOSI */
    gpio_init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio_init);
    
    // connect SPI1 pins to SPI alternate function
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
    
    // configure chip select Pin_A_4 = NSS
    gpio_init.GPIO_Mode = GPIO_Mode_OUT;
    gpio_init.GPIO_Pin = GPIO_Pin_4;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN; //GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &gpio_init);
    //GPIOA->BSRRL |= GPIO_Pin_4; // set high
    
    // configue SPI1 CPOL, CPHA, MASTER, Baud Rate
    spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi_init.SPI_Mode = SPI_Mode_Master;
    spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    spi_init.SPI_DataSize = SPI_DataSize_8b;
    spi_init.SPI_CPOL = SPI_CPOL_Low;
    spi_init.SPI_CPHA = SPI_CPHA_1Edge;
    spi_init.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
    spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI1, &spi_init);
    // Enable SPI1
    SPI_Cmd(SPI1, ENABLE);
    //**************************************************************
    
    //SPI 3 Slave
    // enable clock for used IO pins
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    /* Pin_C_10 = CLK
       Pin_C_11 = MISO
       Pin_C_12 = MOSI */
    gpio_init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &gpio_init);
       
    // connect SPI3 pins to SPI alternate function
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
    
    // configure chip select Pin_A_15 = NSS
    gpio_init.GPIO_Pin = GPIO_Pin_15;
    gpio_init.GPIO_Mode = GPIO_Mode_IN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    /*gpio_init.GPIO_Pin = GPIO_Pin_15;
    gpio_init.GPIO_Mode = GPIO_Mode_AF;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;*/
    GPIO_Init(GPIOA, &gpio_init);
    
    // configue SPI3 CPOL, CPHA, Slave, Baud Rate
    spi_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi_init.SPI_Mode = SPI_Mode_Slave;
    spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    spi_init.SPI_DataSize = SPI_DataSize_8b;
    spi_init.SPI_CPOL = SPI_CPOL_Low;
    spi_init.SPI_CPHA = SPI_CPHA_1Edge;
    spi_init.SPI_NSS = SPI_NSS_Hard;
    spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI3, &spi_init);
    // Enable SPI3
    SPI_Cmd(SPI3, ENABLE);
    
    // Enable the NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    nvic_init.NVIC_IRQChannel = SPI1_IRQn;
    nvic_init.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_init.NVIC_IRQChannelSubPriority = 0;
    nvic_init.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init);
}

void spi_sendData(uint16_t data)
{
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)) // Wait to Tx empty
    {
        
    }
    SPI_I2S_SendData(SPI1, data);
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
    {
        
    }
    SPI_I2S_ReceiveData(SPI1);
}
uint16_t spi_receiveData()
{
    /*while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) // Wait to Rx not empty
    {
        
    }
    SPI_I2S_ReceiveData(SPI3);*/
    while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE)) // Wait to Rx not empty
    {
        
    }
    uint16_t data = SPI_I2S_ReceiveData(SPI3);
    return data;
}