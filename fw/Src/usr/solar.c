#include "main.h"
#include "mini-printf.h"
#include "solar.h"

solar_struct solar;

uint8_t buffer[32];

void solar_print_values(void)
{
    snprintf((char *)buffer, 32, "\033[2J");
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "Battery Voltage: %f\r\n", solar.adc.battery_voltage);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "Battery Current: %f\r\n", solar.adc.battery_current);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "Solar Voltage: %f\r\n", solar.adc.solar_voltage);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "Solar Current: %f\r\n", solar.adc.solar_current);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "Load Current: %f\r\n", solar.adc.load_current);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "DCDC enable: %d\r\n", solar.dcdc.enable);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "DCDC duty: %d\r\n", solar.dcdc.duty);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "Load enable: %d\r\n", solar.load_enable);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "Error: %d\r\n", solar.error);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "ADS1115 ch0: %d\r\n", solar.adc.ads1115_values[0]);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);
    snprintf((char *)buffer, 32, "ADS1115 ch1: %d\r\n", solar.adc.ads1115_values[1]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);
    snprintf((char *)buffer, 32, "ADS1115 ch2: %d\r\n", solar.adc.ads1115_values[2]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);
    snprintf((char *)buffer, 32, "ADS1115 ch3: %d\r\n", solar.adc.ads1115_values[3]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    //snprintf((char *)buffer, 32, "\r\n\r\n\r\n\r\n\r\n\r\n");
    //HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);
}

void solar_command_handler(uint8_t command)
{
    switch(command)
    {
        case COMMAND_DCDC_ENABLE:
                if (solar.dcdc.enable_user)
                    solar.dcdc.enable_user = 0;
                else
                    solar.dcdc.enable_user = 1;
            break;
            
        case COMMAND_PWM_DOWN:
                if(solar.dcdc.duty>0) 
                    solar.dcdc.duty -= 100;
            break;

        case COMMAND_PWM_UP:
                if(solar.dcdc.duty<5000)
                    solar.dcdc.duty += 100;
            break;

        case COMMAND_LOAD_ENABLE:
                if (solar.load_enable)
                    solar.load_enable = 0;
                else
                    solar.load_enable = 1;
            break;
    }
}


void solar_main(void)
{
    uint8_t command;

    solar_comm_init();
    solar_adc_init();
    solar_dcdc_init();
    
    HAL_TIM_Base_Start_IT(&htim2);

    while(1)
    {
        if(solar.tick)
        {
            solar.tick = 0;
            solar_ads1115_read();
            solar_adc_get_values();

            command = solar_comm_receive();
            solar_command_handler(command);
            //if (command) 
            //    solar_print_values();

            
            solar_mppt();

            //solar_dcdc_enable(solar.dcdc.enable_user);
            //solar_dcdc_enable(solar.dcdc.enable);
            //solar_dcdc_set_duty(solar.dcdc.duty);
            solar_load_enable(solar.load_enable);

            if(solar.counter > 50)
            {
                solar.counter = 0;
                //solar_print_values();
                HAL_GPIO_TogglePin(BUILTIN_LED_GPIO_Port, BUILTIN_LED_Pin);
            }
        }
    }
}

void solar_load_enable(uint8_t enable)
{
    if (enable)
    {
        HAL_GPIO_WritePin(SWITCH_LOAD_GPIO_Port, SWITCH_LOAD_Pin,GPIO_PIN_RESET);
    }   
    else
    {
        HAL_GPIO_WritePin(SWITCH_LOAD_GPIO_Port, SWITCH_LOAD_Pin,GPIO_PIN_SET);
    }
}

void solar_50Hz_callback(void)
{
    if(solar.tick == 1)
    {
        solar.error++;
    }
    solar.tick = 1;
    solar.counter++;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim2)
    {
        solar_50Hz_callback();
    }
}
