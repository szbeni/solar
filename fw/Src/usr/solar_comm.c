#include "main.h"
#include "solar.h"
#include "mini-printf.h"

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

void solar_comm_command_handler(uint8_t command)
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
                solar.dcdc.duty -= 100;
                if(solar.dcdc.duty<0)
                    solar.dcdc.duty = 0;

            break;

        case COMMAND_PWM_UP:
                solar.dcdc.duty += 100;
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
                if (solar.fan_speed)
                    solar.fan_speed = 0;
                else
                    solar.fan_speed = 255;
            break;
        case COMMAND_MPPT_ENABLE:
                if (solar.mppt.enable)
                    solar.mppt.enable = 0;
                else
                    solar.mppt.enable = 1;
            break;       
    }
}

void solar_comm_send_values(void)
{
    snprintf((char *)(solar.send_buffer), SOLAR_SEND_BUFFER_SIZE, "SV%fBV%fSC%fBC%fLC%fDD%fDE%dMD%dMV%fMR%dMP%fME%dMS%dLE%dFS%d%EC%d\r\n", solar.adc.solar_voltage, solar.adc.battery_voltage, solar.adc.solar_current, solar.adc.battery_current, solar.adc.load_current, (float)solar.dcdc.duty/50.0, solar.dcdc.enable, solar.mppt.deadtime, solar.mppt.mppt_voltage, solar.mppt.direction, solar.mppt.prev_solar_power, solar.mppt.enable, solar.mppt.state, solar.load_enable&&solar.load_enable_user, solar.fan_speed, solar.error);
    HAL_UART_Transmit(&huart1,solar.send_buffer,strlen((char*)(solar.send_buffer)),0xFFFF);   
}

/*
void solar_print_values(void)
{
    snprintf((char *)buffer, 64, "\033[2J");
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

    snprintf((char *)buffer, 32, "ADS1115 ch1 load: %d\r\n", solar.adc.ads1115_values[1]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "ADS1115 ch2 solar: %d\r\n", solar.adc.ads1115_values[2]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "ADS1115 ch3 bat: %d\r\n", solar.adc.ads1115_values[3]-16384);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 64, "ADS1115 ch2 solar: %d\r\n", solar.adc.ads1115_values[2]+solar.adc.ads1115_offset[2]);
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);

    snprintf((char *)buffer, 32, "\r\n\r\n\r\n\r\n\r\n\r\n");
    HAL_UART_Transmit(&huart1,buffer,strlen((char*)buffer),0xFFFF);
}
*/
