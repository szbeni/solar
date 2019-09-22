#include "solar.h"


void solar_fan_enable(uint8_t enable)
{
    if (enable)
    {
        HAL_GPIO_WritePin(SWITCH_FAN_GPIO_Port, SWITCH_FAN_Pin, GPIO_PIN_SET);
    }   
    else
    {
        HAL_GPIO_WritePin(SWITCH_FAN_GPIO_Port, SWITCH_FAN_Pin, GPIO_PIN_RESET);
    }
}