#include "solar.h"

void solar_fan_init(void)
{
    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

void solar_fan_set_speed(uint8_t speed)
{
    if (speed != solar.fan_speed)
    {
        solar.fan_speed = speed;
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);
    }
        
}