#include "solar.h"

void solar_dcdc_init(void)
{
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
}


static uint8_t dcdc_enabled=1;
uint8_t solar_dcdc_is_enabled(void)
{
    return dcdc_enabled;
}

void solar_dcdc_enable(uint8_t enable)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (dcdc_enabled != enable)
    {
        dcdc_enabled = enable;
        if (enable)
        {
            solar_dcdc_set_duty(0);

            GPIO_InitStruct.Pin = PWM_DCDC_Pin;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(PWM_DCDC_GPIO_Port, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = PWM_DCDC_N_Pin;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(PWM_DCDC_N_GPIO_Port, &GPIO_InitStruct);


        }   
        else
        {
            solar_dcdc_set_duty(0);

            HAL_GPIO_WritePin(PWM_DCDC_GPIO_Port, PWM_DCDC_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PWM_DCDC_N_GPIO_Port, PWM_DCDC_N_Pin, GPIO_PIN_SET);

            GPIO_InitStruct.Pin = PWM_DCDC_Pin;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(PWM_DCDC_GPIO_Port, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = PWM_DCDC_N_Pin;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(PWM_DCDC_N_GPIO_Port, &GPIO_InitStruct);

        }
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