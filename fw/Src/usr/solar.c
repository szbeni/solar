#include "main.h"
#include "solar.h"

solar_struct solar;

void solar_main(void)
{
    uint8_t command;

    solar_comm_init();
    solar_adc_init();
    solar_dcdc_init();
    solar_mppt_init();
    solar_fan_init();

    solar.mppt.enable = 1;
    solar.load_enable = 1;
    solar.load_enable_user = 1;

    solar_dcdc_enable(0);
    solar_load_enable(0);
    solar_fan_set_speed(0);

    HAL_Delay(200);
    solar_ads1115_reset_offsets();

    //start 50Hz solar main timer
    HAL_TIM_Base_Start_IT(&htim2);

    while(1)
    {
        if(solar.tick)
        {
            //clear 50Hz tick
            solar.tick = 0;

            //read adc values
            if (solar_ads1115_read() != HAL_OK)
            {
                solar_ads1115_reinit();
                solar.mppt.state = SOLAR_MPPT_STATE_OFF;
                solar.dcdc.enable = 0;
                solar.dcdc.duty = 0;
            }

            solar_adc_get_values();

            //receive commands
            command = solar_comm_receive();
            solar_comm_command_handler(command);

            //do the MPPT control
            solar_mppt();

            //check overvoltage from Solar controller, if DCDC switch if faulty switch off the Pmosfet fior 5 sec
            uint8_t dcdc_safety_enable = 1;
            static uint16_t overvoltage_counter = 0;
            if (solar.adc.battery_voltage_avg > SOLAR_MPPT_BATTERY_VOLTAGE_CRITICAL) overvoltage_counter = 500;
            if(overvoltage_counter > 0)
            {
                //switch back on after the timeout
                overvoltage_counter--;
                dcdc_safety_enable = 0;
                solar.dcdc.duty = 0;
            }
            
            solar_dcdc_set_duty(solar.dcdc.duty);
            solar_dcdc_enable(dcdc_safety_enable && solar.dcdc.enable); 

            

            //add a hysteresis for swith on and off voltages
            if(solar.load_enable_deadtime>0)
                solar.load_enable_deadtime--;

            //check if loads needs to be enabled or disabled
            if (solar.load_enable == 1)
            {
                //set at what voltage the batter shuts off
                float switch_off_threshold_voltage = SOLAR_BATTERY_LOAD_SWITCH_OFF_VOLTAGE;
                //if there is a high load decrease the shut off voltage (for diesel heater startup)
                if (solar.adc.battery_current > SOLAR_BATTERY_LOAD_SWITCH_OFF_HIGH_LOAD_CURRENT)
                    switch_off_threshold_voltage = SOLAR_BATTERY_LOAD_SWITCH_OFF_HIGH_LOAD_VOLTAGE;

                //check for overvoltage
                if(solar.adc.battery_voltage > SOLAR_BATTERY_LOAD_SWITCH_DANGER_VOLTAGE)
                {
                    //voltage need to be over for more than threshold cycles
                    if(++solar.load_enable_counter >= SOLAR_BATTERY_LOAD_SWITCH_COUNTER_THRESHOLD)
                    {
                        solar.load_enable_counter = 0;
                        solar.load_enable = 0;
                        solar.load_enable_deadtime = SOLAR_BATTERY_LOAD_SWITCH_DEADTIME;
                    }
                }
                //check for undervoltage
                else if (solar.adc.battery_voltage < switch_off_threshold_voltage)
                {
                    //voltage needs to be under for more than threshold cycles
                    if(++solar.load_enable_counter >= SOLAR_BATTERY_LOAD_SWITCH_COUNTER_THRESHOLD)
                    {
                        solar.load_enable_counter = 0;
                        solar.load_enable = 0;
                        solar.load_enable_deadtime = SOLAR_BATTERY_LOAD_SWITCH_DEADTIME;
                    }
                }
                else
                {
                    solar.load_enable_counter = 0;
                }
            }
            else if (solar.load_enable == 0)
            {
                //Check if voltage is above to switch back the load
                if (solar.adc.battery_voltage > SOLAR_BATTERY_LOAD_SWITCH_ON_VOLTAGE && solar.adc.battery_voltage < SOLAR_BATTERY_LOAD_SWITCH_DANGER_VOLTAGE)
                {
                    if(solar.load_enable_deadtime == 0)
                    {
                        if(++solar.load_enable_counter >= SOLAR_BATTERY_LOAD_SWITCH_COUNTER_THRESHOLD)
                        {
                            solar.load_enable_counter = 0;
                            solar.load_enable = 1;
                            solar.load_enable_deadtime = SOLAR_BATTERY_LOAD_SWITCH_DEADTIME;
                        }
                    }
                }
                else
                {
                    solar.load_enable_counter = 0;
                }
            }

            //enable/disable load
            solar_load_enable(solar.load_enable_user && solar.load_enable);
            
            //send values every 5th loop (10Hz)
            if((solar.counter % 5) == 0)
                solar_comm_send_values();

            //toggle LED every 50th loop (1Hz)
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
