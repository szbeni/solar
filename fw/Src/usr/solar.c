#include "main.h"
#include "mini-printf.h"
#include "solar.h"

solar_struct solar;

uint8_t buffer[92];

void solar_print_values(void)
{
    //snprintf((char *)buffer, 64, "\033[2J");
    //HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    /*snprintf((char *)buffer, 32, "Battery Voltage: %f\r\n", solar.adc.battery_voltage);
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

    snprintf((char *)buffer, 32, "ADS1115 ch1 load: %d\r\n", solar.adc.ads1115_values[1]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "ADS1115 ch2 solar: %d\r\n", solar.adc.ads1115_values[2]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "ADS1115 ch3 bat: %d\r\n", solar.adc.ads1115_values[3]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

*/

    //snprintf((char *)buffer, 64, "ADS1115 ch2 solar: %d\r\n", solar.adc.ads1115_values[2]+solar.adc.ads1115_offset[2]);
    //HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);


    snprintf((char *)buffer, 92, "SV%f BV%f SC%f BC%f LC%f D%d E%d DT%d MV%f SI%f\r\n", solar.adc.solar_voltage, solar.adc.battery_voltage, solar.adc.solar_current, solar.adc.battery_current, solar.adc.load_current, solar.dcdc.duty, solar.dcdc.enable, solar.mppt.deadtime, solar.mppt.mppt_voltage, solar.mppt.stateI);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    
    //snprintf((char *)buffer, 32, "\r\n\r\n\r\n\r\n\r\n\r\n");
    //HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);
}

void solar_command_handler(uint8_t command)
{
    switch(command)
    {
        case COMMAND_DCDC_ENABLE:
                if (solar.dcdc.enable)
                    solar.dcdc.enable = 0;
                else
                    solar.dcdc.enable = 1;
            break;
            
        case COMMAND_PWM_DOWN:
                solar.dcdc.duty -= 10;
                if(solar.dcdc.duty<0)
                    solar.dcdc.duty = 0;

            break;

        case COMMAND_PWM_UP:
                solar.dcdc.duty += 10;
                if(solar.dcdc.duty>SOLAR_DCDC_MAX_DUTY)
                    solar.dcdc.duty = SOLAR_DCDC_MAX_DUTY;
                
            break;

        case COMMAND_LOAD_ENABLE:
                if (solar.load_enable_user)
                    solar.load_enable_user = 0;
                else
                    solar.load_enable_user = 1;
            break;

        case COMMAND_FAN_ENABLE:
                if (solar.fan_enable)
                    solar.fan_enable = 0;
                else
                    solar.fan_enable = 1;
            break;
        case COMMAND_MPPT_ENABLE:
                if (solar.mppt.enable)
                    solar.mppt.enable = 0;
                else
                    solar.mppt.enable = 1;
            break;
                
    }
}


void solar_main(void)
{
    uint8_t command;

    solar_comm_init();
    solar_adc_init();
    solar_dcdc_init();
    solar_mppt_init();
    
    HAL_TIM_Base_Start_IT(&htim2);

    solar.mppt.enable = 1;
    solar.load_enable = 1;
    solar.load_enable_user = 1;

    solar_dcdc_enable(0);
    solar_load_enable(0);
    solar_fan_enable(0);

    HAL_Delay(200);
    solar_ads1115_reset_offsets();

    uint8_t dir=0;
    int16_t counter=0;
    while(1)
    {
        if(solar.tick)
        {
            solar.tick = 0;
            if(solar.counter > 50)
            {
                //1 sec
                solar.counter = 0;                  
            }
            if (dir)
                counter+=20;
            else
                counter-=20;
            
            if (counter > 5000)
            {
                counter = 5000;
                dir = 0;
            }
            if (counter < 0)
            {
                counter = 0;
                dir = 1;
            }
                
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, counter);


        }
    }


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
            //solar_dcdc_enable(solar.dcdc.enable_user);
            //solar_dcdc_enable(solar.dcdc.enable);
            //solar_dcdc_set_duty(solar.dcdc.duty);
            //if ((solar.counter % 2) == 0)
            //    solar.fan_enable = 1;
            //else
            //    solar.fan_enable = 0;
                
            solar.mppt.prev_solar_power = solar.adc.solar_current * solar.adc.solar_voltage;

            //if((solar.counter % 10) == 0)
            //{
            if (solar.mppt.enable)
                solar_mppt();
            


            solar_dcdc_set_duty(solar.dcdc.duty);
            solar_dcdc_enable(solar.dcdc.enable); 
            //solar_fan_enable(solar.fan_enable);

            //add a hysteresis for swith on and off voltages
            if (solar.load_enable == 1)
            {
                if (solar.adc.battery_voltage < SOLAR_BATTERY_MIN_VOLTAGE)
                    solar.load_enable = 0;
            }
            else if (solar.load_enable == 0)
            {
                if (solar.adc.battery_voltage > SOLAR_BATTERY_VOLTAGE_SWITCH_ON_LOAD)
                    solar.load_enable = 1;
            }
            solar_load_enable(solar.load_enable_user && solar.load_enable);
            
            if((solar.counter % 5) == 0)
                solar_print_values();
            if(solar.counter > 50)
            {
                solar.counter = 0;
                
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
