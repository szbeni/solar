#include "solar.h"

void solar_dcdc_init(void)
{
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
}

uint8_t solar_dcdc_is_enabled(void)
{
    if (HAL_GPIO_ReadPin(SWITCH_DCDC_GPIO_Port, SWITCH_DCDC_Pin) == GPIO_PIN_SET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void solar_dcdc_enable(uint8_t enable)
{
    if (enable)
    {
        HAL_GPIO_WritePin(SWITCH_DCDC_GPIO_Port, SWITCH_DCDC_Pin,GPIO_PIN_SET);
    }   
    else
    {
        HAL_GPIO_WritePin(SWITCH_DCDC_GPIO_Port, SWITCH_DCDC_Pin,GPIO_PIN_RESET);
    }
}

void solar_dcdc_set_duty(int16_t duty)
{
    if (duty > 5000)
        duty = 5000;
    else if (duty < 0)
        duty = 0;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
}