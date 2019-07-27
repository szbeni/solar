#include "main.h"
#include "mini-printf.h"
#include "solar.h"

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;


solar_struct solar;

float ADC_Convert_SolarVoltage(uint16_t raw)
{
    float scaled;
    scaled = 3.3 / 4095.0 * (20.0 + 1.8) / 1.8 * raw;
    return scaled;
}

float ADC_Convert_SolarCurrent(uint16_t raw)
{
    //ACS712 30A version 2.5V + 0.066 Volt/Amps (bi directional)
    #define ADC_CALIB_SOLAR_CURRENT_CENTRE 1968     //ideally 2048

    float scaled;
    scaled = (raw - ADC_CALIB_SOLAR_CURRENT_CENTRE) * 3.3 / 4095.0 * 0.066;
    return scaled;
}

void GetADCValues()
{
    solar.solarVoltage = ADC_Convert_SolarVoltage(solar.adcRaw[0]);
    solar.solarCurrent = ADC_Convert_SolarCurrent(solar.adcRaw[1]);
}

void solar_main(void)
{
    uint8_t buffer[32];
    //solar_comm_init();
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)solar.adcRaw, 5);
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);



    uint16_t counter = 0;
    uint16_t pwm_duty = 0;
    uint8_t uart_char;

    while(1)
    {
        if(HAL_UART_Receive(&huart1, &uart_char, 1, 0) == HAL_OK)
        {
            if(uart_char == '1')
            {
                HAL_GPIO_TogglePin(SWITCH_SOLAR_GPIO_Port, SWITCH_SOLAR_Pin);
            }
            else if( (uart_char == '2') || (uart_char == '3') )
            {
                if(uart_char == '3')
                {
                    pwm_duty += 100;
                }    
                else{
                    pwm_duty -= 100;
                }
                __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_duty);   
                snprintf((char *)buffer, 32, "PWM Duty: %d\r\n", pwm_duty);
                HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);
            }
          

        }
        if(++counter == 1000)
        {
            counter = 0;
            GetADCValues();



            snprintf((char *)buffer, 32, "Solar Voltage: %f\r\n", solar.solarVoltage);
            HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

            snprintf((char *)buffer, 32, "Solar Current: %f\r\n", solar.solarCurrent);
            HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

            HAL_GPIO_TogglePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin);
        }

        HAL_Delay(1);
    }

}