#include "main.h"
#include "solar.h"

extern UART_HandleTypeDef huart1;
uint8_t uart_char;

void solar_comm_char_received(uint8_t c);

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t tmp = uart_char;
    solar_comm_char_received(tmp);
    //HAL_UART_Receive_IT(&huart1, &uart_char, 1);
}

void solar_comm_init(void)
{
    //HAL_UART_Receive_IT(&huart1, &uart_char, 1);
}

void solar_comm_char_received(uint8_t c)
{
    if(c == '1')
        HAL_GPIO_TogglePin(SWITCH_SOLAR_GPIO_Port, SWITCH_SOLAR_Pin);

    //HAL_UART_Transmit(&huart1, "toggle\r\n",8,0xFFFF);
}