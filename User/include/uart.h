/*
 * Date: 25/4/2016
 * Module: lcd
 * Writer: ESS
 * Description: This file contains uart function declarations
 */

// Macro declarations
#define TC_BIT 0x0040
#define TC_SHIFT_LEFT 6

#define RXNE_BIT 0x0020
#define RXNE_SHIFT_LEFT 5

// Function declarations

/* Name: uart3_init_interrupt
 * Module: uart
 * Parameters: void
 * Return: void
 * Description: Initialize UART 3
 */
void uart3_init_interrupt(void);

/* End of file */
