#include "solar.h"



void solar_mppt_init(void)
{
    solar.mppt.kI = 0.1;
    solar.mppt.kP = 1.5;
    solar.mppt.limitI = 100;
    solar.mppt.stateI = 0;
}

void solar_mppt(void)
{

    float solar_power = 0;
    //when DCDC is off
    if (!solar_dcdc_is_enabled())
    {
        solar.dcdc.enable = 0;
        solar.fan_enable = 0;
        //check if there is sun, solar panel voltage is above minimum level
        if (solar.adc.solar_voltage > SOLAR_PANEL_VOLTAGE_MIN)
        {
            //check if deadtime is over
            if (solar.mppt.deadtime  == 0)
            {
                //enable dcdc
                solar.dcdc.enable = 1;
                solar.mppt.deadtime = SOLAR_MPPT_DEADTIME_ON;

            }
            else
            {
                solar.mppt.deadtime--;                
            }
        } 
    }
    //when DCDC is on
    else
    {
        solar_power = solar.adc.solar_current * solar.adc.solar_voltage;
        solar.fan_enable = 0;
        
            
        //check if solar panel is drawing current from the battery
        if (solar.mppt.deadtime == 0 && (solar.adc.solar_current < 0.2 || solar_power < 1))
        {
            solar.dcdc.duty -= SOLAR_MPPT_DUTY_STEP_BIG;
            //if duty cycle is at min and current is still negative sun must have been gone
            if (solar.dcdc.duty <= 0)
            {
                //stop solar panel draning the battery
                solar.dcdc.enable = 0;
                //not allowed to change enable state for the deadtime
                solar.mppt.deadtime = SOLAR_MPPT_DEADTIME_OFF;
            }
        }
        else
        {
            
            if (solar_power > 20)
                solar.fan_enable = 1;

            if (solar.dcdc.duty == 0)
            {
                solar.mppt.solar_voltage_oc = solar.adc.solar_voltage;
                solar.mppt.mppt_voltage = 0.9 * solar.adc.solar_voltage;
            }
                

            if(solar.adc.solar_voltage > solar.mppt.solar_voltage_oc)
                solar.mppt.solar_voltage_oc = solar.adc.solar_voltage;

            solar.mppt.power_sum += solar_power;
            if(++solar.mppt.counter == 100)
            {
                float power_average = solar.mppt.power_sum / 100.0;
                solar.mppt.counter = 0;
                solar.mppt.power_sum = 0;
                //if current power is greater than previous keep going the same way
                if (power_average > solar.mppt.power_average)
                {
                    
                    if(solar.mppt.direction == SOLAR_MPPT_DIR_UP)
                        solar.mppt.mppt_voltage += 0.4;
                    else
                        solar.mppt.mppt_voltage -= 0.4;
                }
                //if current power is less than previous go the other way
                else
                {
                    if(solar.mppt.direction == SOLAR_MPPT_DIR_UP)
                    {
                        solar.mppt.mppt_voltage -= 0.4;
                        solar.mppt.direction = SOLAR_MPPT_DIR_DOWN;
                    }
                    else
                    {
                        solar.mppt.mppt_voltage += 0.4;
                        solar.mppt.direction = SOLAR_MPPT_DIR_UP;
                    }
                }
                /*if(solar.mppt.mppt_scaler < 0.8)
                    solar.mppt.mppt_scaler = 0.8;
                else if (solar.mppt.mppt_scaler > 1)
                    solar.mppt.mppt_scaler = 1;

                //solar.mppt.mppt_voltage = solar.mppt.mppt_scaler * solar.mppt.solar_voltage_oc;
                */
                if(solar.mppt.mppt_voltage < SOLAR_PANEL_VOLTAGE_MIN)
                    solar.mppt.mppt_voltage = SOLAR_PANEL_VOLTAGE_MIN;
                else if (solar.mppt.mppt_voltage > SOLAR_PANEL_VOLTAGE_MAX)
                    solar.mppt.mppt_voltage = SOLAR_PANEL_VOLTAGE_MAX;
                
                solar.mppt.power_average = power_average;
            }
            float error = solar.mppt.mppt_voltage - solar.adc.solar_voltage;

            //if (solar.adc.battery_voltage > 12.8)
            //{
            //error = -1 - solar.adc.battery_current ;
            //solar.mppt.kP = 10;
            //solar.mppt.kI = 0;
            //}            
            
            solar.mppt.stateI += error;
            
            if (solar.mppt.stateI > solar.mppt.limitI)
                solar.mppt.stateI = solar.mppt.limitI;
            else if (solar.mppt.stateI < -solar.mppt.limitI)  
                solar.mppt.stateI = -solar.mppt.limitI;

            solar.dcdc.duty -= error * solar.mppt.kP + solar.mppt.kI * solar.mppt.stateI;                
        

                
        }
        if (solar.mppt.deadtime > 0)
            solar.mppt.deadtime--;            

    }

    if (solar.dcdc.duty > SOLAR_DCDC_MAX_DUTY)
        solar.dcdc.duty = SOLAR_DCDC_MAX_DUTY;
    else if(solar.dcdc.duty < 0)
        solar.dcdc.duty = 0;


    solar.mppt.prev_solar_power = solar_power;
}