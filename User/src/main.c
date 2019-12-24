#include "stm32f4xx.h"
#include "uart.h"
#include "lcd.h"
#include "queue.h"
#include "string.h"
#include "io.h"
#include "timer.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Define Macros
#define STR_MAX_LENGTH 100

// Global variables
volatile char* gp_input_char_msg     = "Choose your option (1,2, ..): \r\n1. Student info\r\n2. Basic operation\r\n3. Simple led\r\n4. Advance led\r\n5. Timer\r\n6. ...\r\n";
volatile char* gp_dont_support_str   = "Don't support";
volatile char* gp_new_line_str       = "\r\n";
volatile char* gp_debug              = "Debug !!\r\n";
volatile char  gp_char_input         = 0;
char gp_expression[20];

// Init function
void delay(int ms);
void student_function(void);
void operation_function(void);
void simple_led_function(void);
void advance_led_function(void);
void timer_function(void);
char* basic_operation(char*);
int calculator(char*);
void get_expression(void);

// Declare data structure
queue_t g_queue_send;
queue_t g_queue_receive;

int main()
{
    // Configure Clock
    RCC_DeInit();

    // Initialize IO
    io_init();
    
    // Initialize UART
    uart3_init_interrupt();

    // Initialize queue
    queue_init(&g_queue_send);
    queue_init(&g_queue_receive);

    // Push " Input your chracter: " to queue and then ignite sending interrupt
    queue_push_string(&g_queue_send, (char*)gp_input_char_msg, strlen((char*)gp_input_char_msg));
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

    for (; ; )
    {
        if (!queue_is_empty(&g_queue_receive))
        {
            gp_char_input = queue_pop(&g_queue_receive);
            switch(gp_char_input)
            {
               case 49: // students infomation
                   student_function();
                   break;
               case 50: // operation
                   operation_function();    
                   break;
               case 51: // simple_led
                   simple_led_function();
                   break;
               case 52: // advanced_led
                   break;
               case 53: // timer
                   timer_function();
                   break;
               default:
                   queue_push_string(&g_queue_send, (char*)gp_dont_support_str, strlen((char*)gp_dont_support_str));
                   queue_push_string(&g_queue_send, (char*)gp_new_line_str, strlen((char*)gp_new_line_str));
                   USART_ITConfig(USART3, USART_IT_TXE, ENABLE);     

            }
            /*// Examine character
            if ((gp_char_input >= 32) && (gp_char_input <= 126))
            {
                // Output to LCD that it is visible character
                LCD_DisplayStringLine(0, (uint8_t*)gp_visible_char_str);

                // Output to HT this character
                queue_push(&g_queue_send, gp_char_input);
                queue_push_string(&g_queue_send, (char*)gp_new_line_str, strlen((char*)gp_new_line_str));
            }
            else
            {
                // Output to LCD that it is not visible character
                LCD_DisplayStringLine(0, (uint8_t*)gp_invisible_char_str);

                // Output to HT the message "Don't support"
                queue_push_string(&g_queue_send, (char*)gp_dont_support_str, strlen((char*)gp_dont_support_str));
                queue_push_string(&g_queue_send, (char*)gp_new_line_str, strlen((char*)gp_new_line_str));
            }

            queue_push_string(&g_queue_send, (char*)gp_input_char_msg, strlen((char*)gp_input_char_msg));
            USART_ITConfig(USART3, USART_IT_TXE, ENABLE);*/
        }
    }
}

//--------------------------INTERRUPT HANDLER----------------------------

void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_TXE) == SET)
    {
        // Send characters until queue is empty
        if (!queue_is_empty(&g_queue_send))
        {
            USART_SendData(USART3, queue_pop(&g_queue_send));
        }
        else
        {
            USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
        }
    }

    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
        queue_push(&g_queue_receive, (char)USART_ReceiveData(USART3));
    }
}

void TIM2_IRQHandler()
{
    // Checks whether the TIM2 interrupt has occurred or not
    if (TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        GPIO_ToggleBits(LED_PORT, LED_RED);
        
        // Clears the TIM2 interrupt pending bit
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
//-----------------------------FUNCTION------------------------------
void student_function()
{
    char* p_student = " 1. Student info \r\n ID: 16520179 \r\n Full name: Nguyen Thanh Danh \r\n ID: 16520154 \r\n Full name: Nguyen Van Cuong \r\n ESC: return previous menu\r\n";
    queue_push_string(&g_queue_send, (char*)p_student, strlen((char*)p_student));
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    for(; ; )
    {
        if (!queue_is_empty(&g_queue_receive))
        {
            gp_char_input = queue_pop(&g_queue_receive);
            if (gp_char_input == 27)
            {
                queue_push_string(&g_queue_send, (char*)gp_input_char_msg, strlen((char*)gp_input_char_msg));
                USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                return;
            }
            else
            {

            }
        }
    }
}
void operation_function()
{
    char* p_operation = "2. Basic operation (a, b,...)\r\na. Plus\r\nb. Subtract\r\nc. Multiple\r\nd. Divide \r\ne. Module\r\nESC: return previos menu\r\n";
    queue_push_string(&g_queue_send, (char*)p_operation, strlen((char*)p_operation));
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    char* p_plus = "a. Plus\r\n";
    char* p_sub = "b. Subtract\r\n";
    char* p_mul = "c. Multiple\r\n";
    char* p_divide = "d. Divide\r\n";
    //char* p_mod = "e. Module\r\n";
    char* p_exp = "Expresion: ";
    char* p_result = "\r\nResult: ";
    char* p_esc = "\r\nESC: return previos menu\r\n";
    int result;
    char* p_result_exp;
    char convertIntToChar[20];
    for(; ; )
    {
        if(!queue_is_empty(&g_queue_receive))
        {
            gp_char_input = queue_pop(&g_queue_receive);
            switch(gp_char_input)
            {
                case 27:
                    queue_push_string(&g_queue_send, (char*)gp_input_char_msg, strlen((char*)gp_input_char_msg));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    return;
                case 97:
                    queue_push_string(&g_queue_send, (char*)p_plus, strlen((char*)p_plus));
                    queue_push_string(&g_queue_send, (char*)p_exp, strlen((char*)p_exp));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    get_expression();
                    result = calculator(gp_expression);
                    sprintf(convertIntToChar,"%d", result);
                    p_result_exp = convertIntToChar;
                    queue_push_string(&g_queue_send, (char*)p_result, strlen((char*)p_result));
                    queue_push_string(&g_queue_send, (char*)p_result_exp, strlen((char*)p_result_exp));
                    queue_push_string(&g_queue_send, (char*)p_esc, strlen((char*)p_esc));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    for(; ;)
                    {
                        if(!queue_is_empty(&g_queue_receive))
                        {
                            gp_char_input = queue_pop(&g_queue_receive);
                            if (gp_char_input == 27)
                            {
                                queue_push_string(&g_queue_send, (char*)p_operation, strlen((char*)p_operation));
                                USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                                break;
                            }
                            else
                            {
                                
                            }
                        }
                    }       
                    break;
                case 98:
                    queue_push_string(&g_queue_send, (char*)p_sub, strlen((char*)p_sub));
                    queue_push_string(&g_queue_send, (char*)p_exp, strlen((char*)p_exp));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    get_expression();
                    result = calculator(gp_expression);
                    sprintf(convertIntToChar,"%d", result);
                    p_result_exp = convertIntToChar;
                    queue_push_string(&g_queue_send, (char*)p_result, strlen((char*)p_result));
                    queue_push_string(&g_queue_send, (char*)p_result_exp, strlen((char*)p_result_exp));
                    queue_push_string(&g_queue_send, (char*)p_esc, strlen((char*)p_esc));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    for(; ;)
                    {
                        if(!queue_is_empty(&g_queue_receive))
                        {
                            gp_char_input = queue_pop(&g_queue_receive);
                            if (gp_char_input == 27)
                            {
                                queue_push_string(&g_queue_send, (char*)p_operation, strlen((char*)p_operation));
                                USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                                break;
                            }
                            else
                            {
                                
                            }
                        }
                    }       
                    break;
                case 99:
                    queue_push_string(&g_queue_send, (char*)p_mul, strlen((char*)p_mul));
                    queue_push_string(&g_queue_send, (char*)p_exp, strlen((char*)p_exp));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    get_expression();
                    result = calculator(gp_expression);
                    sprintf(convertIntToChar,"%d", result);
                    p_result_exp = convertIntToChar;
                    queue_push_string(&g_queue_send, (char*)p_result, strlen((char*)p_result));
                    queue_push_string(&g_queue_send, (char*)p_result_exp, strlen((char*)p_result_exp));
                    queue_push_string(&g_queue_send, (char*)p_esc, strlen((char*)p_esc));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    for(; ;)
                    {
                        if(!queue_is_empty(&g_queue_receive))
                        {
                            gp_char_input = queue_pop(&g_queue_receive);
                            if (gp_char_input == 27)
                            {
                                queue_push_string(&g_queue_send, (char*)p_operation, strlen((char*)p_operation));
                                USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                                break;
                            }
                            else
                            {
                                
                            }
                        }
                    }       
                    break;
                case 100:                    
                    queue_push_string(&g_queue_send, (char*)p_divide, strlen((char*)p_divide));
                    queue_push_string(&g_queue_send, (char*)p_exp, strlen((char*)p_exp));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    get_expression();
                    result = calculator(gp_expression);
                    if (result == 999999999)
                    {
                        p_result_exp = "Expression Invalid !!";
                    }
                    else
                    {
                        sprintf(convertIntToChar,"%d", result);
                        p_result_exp = convertIntToChar;
                    }
                    queue_push_string(&g_queue_send, (char*)p_result, strlen((char*)p_result));
                    queue_push_string(&g_queue_send, (char*)p_result_exp, strlen((char*)p_result_exp));
                    queue_push_string(&g_queue_send, (char*)p_esc, strlen((char*)p_esc));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    for(; ;)
                    {
                        if(!queue_is_empty(&g_queue_receive))
                        {
                            gp_char_input = queue_pop(&g_queue_receive);
                            if (gp_char_input == 27)
                            {
                                queue_push_string(&g_queue_send, (char*)p_operation, strlen((char*)p_operation));
                                USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                                break;
                            }
                            else
                            {
                                
                            }
                        }
                    }       
                    break;
                default:
                    queue_push_string(&g_queue_send, (char*)gp_dont_support_str, strlen((char*)gp_dont_support_str));
                    queue_push_string(&g_queue_send, (char*)gp_new_line_str, strlen((char*)gp_new_line_str));
                    queue_push_string(&g_queue_send, (char*)p_esc, strlen((char*)p_esc));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    break;
            }
        }
    }
}
void simple_led_function(void)
{
    char* instruction = "3. Simple led\r\n - Press key r to make led red on\r\n - Press key R to make led red off\r\n - Press key g to make led green on\r\n - Press key G to make led green off\r\n ESC: return previos menu\r\n";
    char* led_red_on = "Led red is on !!\r\n";
    char* led_red_off = "Led red is off !!\r\n";
    char* led_green_on = "Led green is on !!\r\n";
    char* led_green_off = "Led green is off !!\r\n";
    queue_push_string(&g_queue_send, (char*)instruction, strlen((char*)instruction));
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    for (; ;)
    {
        if (!queue_is_empty(&g_queue_receive))
        {
            gp_char_input = queue_pop(&g_queue_receive);
            switch(gp_char_input)
            {
                case 114: // r -> led red on
                    led_on(LED_RED);
                    queue_push_string(&g_queue_send, (char*)led_red_on, strlen((char*)led_red_on));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    break;
                case 82: // R -> led red off
                    led_off(LED_RED);
                    queue_push_string(&g_queue_send, (char*)led_red_off, strlen((char*)led_red_off));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    break;
                case 103: // g -> led green on
                    led_on(LED_GREEN);
                    queue_push_string(&g_queue_send, (char*)led_green_on, strlen((char*)led_green_on));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    break;
                case 71: // G -> led green off
                    led_off(LED_GREEN);
                    queue_push_string(&g_queue_send, (char*)led_green_off, strlen((char*)led_green_off));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    break;
                case 27:
                    queue_push_string(&g_queue_send, (char*)gp_input_char_msg, strlen((char*)gp_input_char_msg));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    return;
                default:
                    queue_push_string(&g_queue_send, (char*)gp_dont_support_str, strlen((char*)gp_dont_support_str));
                    queue_push_string(&g_queue_send, (char*)gp_new_line_str, strlen((char*)gp_new_line_str));
                    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                    break;
            }
        }       
    }   
}

void get_expression()
{
    int i = 0;
    for(; ; )
    {
        if (!queue_is_empty(&g_queue_receive))
        {
            gp_char_input = queue_pop(&g_queue_receive);
            if (gp_char_input == 46)
            {
                gp_expression[i] = gp_char_input;
                i++;
                queue_push(&g_queue_send, gp_char_input);
                USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                return;
            }
            else if ((gp_char_input >= 48 && gp_char_input <= 57) || gp_char_input == 42 || gp_char_input == 43 || gp_char_input == 45 || gp_char_input == 47 || gp_char_input == 32)
            {
                // Write your code here
                gp_expression[i] = gp_char_input;
                i++;
                queue_push(&g_queue_send, gp_char_input);
                USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
            }
            else
            {
                
            }
        }
    }
}
void timer_function()
{
    char* timer_instruction = "5. Timer \r\n Press c to change the speed of led \r\n ESC: return previos menu \r\n";
    queue_push_string(&g_queue_send, (char*)timer_instruction, strlen((char*)timer_instruction));
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    for(; ;)
    {
       if (!queue_is_empty(&g_queue_receive))
        {
            gp_char_input = queue_pop(&g_queue_receive);
            if (gp_char_input == 27) 
            {
                TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
                TIM_Cmd(TIM2, DISABLE);
                queue_push_string(&g_queue_send, (char*)gp_input_char_msg, strlen((char*)gp_input_char_msg));
                USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
                return;
            }
            else
            {
                
            }
        }
    }
}
//--------------------------MATH FUNCTION----------------------------
// tinh chieu dai chuoi
int length(char arr[]){
    int i = 0;
    while(arr[i] != 46){
        i++;
    }
    return i;
}
// chuyen tu char sang int
int convert(char ar[], int start, int end){
    int number = 0;
    int sign = 0;
    if (ar[start] == 45)
    {
        sign = -1;
        int i;
        for (i = start + 1; i <= end ; i++)
        {
            switch (ar[i])
            {
            case 48:
                number = number * 10 + 0;
                break;
            case 49:
                number = number * 10 + 1;
                break;
            case 50:
                number = number * 10 + 2;
                break;
            case 51:
                number = number * 10 + 3;
                break;
            case 52:
                number = number * 10 + 4;
                break;
            case 53:
                number = number * 10 + 5;
                break;
            case 54:
                number = number * 10 + 6;
                break;
            case 55:
                number = number * 10 + 7;
                break;
            case 56:
                number = number * 10 + 8;
                break;
            case 57:
                number = number * 10 + 9;
                break;
            default:
                break;
            }
        }
    }
    else
    {
        sign = 1;
        int i;
        for (i = start; i <= end ; i++)
        {
            switch (ar[i])
            {
            case 48:
                number = number * 10 + 0;
                break;
            case 49:
                number = number * 10 + 1;
                break;
            case 50:
                number = number * 10 + 2;
                break;
            case 51:
                number = number * 10 + 3;
                break;
            case 52:
                number = number * 10 + 4;
                break;
            case 53:
                number = number * 10 + 5;
                break;
            case 54:
                number = number * 10 + 6;
                break;
            case 55:
                number = number * 10 + 7;
                break;
            case 56:
                number = number * 10 + 8;
                break;
            case 57:
                number = number * 10 + 9;
                break;
            default:
                break;
            }
        }
    }
    number = number * sign;
    return number;
}
int calculator(char a[]){
    int num1 = 0;
    int num2 = 0;
    int sign = 0;
    int size = length(a);
    int i;
    for(i = 0; i < size; i++){
        if (a[i] == 32)
        {
            num1 = convert(a, 0, i - 1);
            num2 = convert(a, i + 3, size -1);
            sign = a[i+1];
            break;
        }
    }
    switch (sign)
    {
    case 43:
        return num1 + num2;
    case 45:
        return num1 - num2;
    case 42:
        return num1 * num2;
    case 47:
        if (num2 == 0)
            return 999999999; // ma loi mau bang 0
        return num1 / num2;
    default:
        return 111111111; // ma loi khong tim duoc dau 
    }
}
/* End of file */
