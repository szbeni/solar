#include "solar.h"

void solar_dcdc_init(void)
{
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void solar_dcdc_enable(uint8_t enable)
{
    if (enable)
    {
        HAL_GPIO_WritePin(SWITCH_SOLAR_GPIO_Port, SWITCH_SOLAR_Pin,GPIO_PIN_SET);
    }   
    else
    {
        HAL_GPIO_WritePin(SWITCH_SOLAR_GPIO_Port, SWITCH_SOLAR_Pin,GPIO_PIN_RESET);
    }
}

void solar_dcdc_set_duty(uint16_t duty)
{
    if (duty > 5000)
        duty = 5000;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
}