#include "main.h"
#include "solar.h"

extern UART_HandleTypeDef huart1;

void solar_comm_init(void)
{
}

uint8_t solar_comm_receive(void)
{
    uint8_t uart_char;
    if(HAL_UART_Receive(&huart1, &uart_char, 1, 0) == HAL_OK)
    {
          if(uart_char == '1') return COMMAND_DCDC_ENABLE;
          else if(uart_char == '2') return COMMAND_PWM_DOWN;
          else if(uart_char == '3') return COMMAND_PWM_UP;
          else if(uart_char == '4') return COMMAND_LOAD_ENABLE;
          else if(uart_char == '5') return COMMAND_FAN_ENABLE;
          else if(uart_char == '6') return COMMAND_MPPT_ENABLE;



    }
    return 0;
}
