#include "stdint.h"

/* Name: spi1_init
 * Module: spi
 * Parameters: void
 * Return: void
 * Description: Initialize SPI 1
 */
 
 
void spi_Init(void);
void spi_sendData(uint16_t data);
uint16_t spi_receiveData(void);
