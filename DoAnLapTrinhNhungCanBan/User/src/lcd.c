/*
 * Date: 25/4/2016
 * Writer: ESS
 * Description: This file contains LCD function implementations
 */

#include "lcd.h"

void lcd_init(void)
{
    /* LCD initialization */
    LCD_Init();
    
    /* LCD layer initialization */
    LCD_LayerInit();
    
    /* Enable the LTDC */
    LTDC_Cmd(ENABLE);
    
    /* Set LCD foreground layer */
    LCD_SetLayer(LCD_FOREGROUND_LAYER);
    
    /* Set backcolor and text color */
    LCD_SetColors(LCD_COLOR_BLACK, LCD_COLOR_WHITE);
    
    /* Clear LCD */
    LCD_Clear(LCD_COLOR_WHITE);
}

/* End of file */
