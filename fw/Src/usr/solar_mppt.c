#include "solar.h"





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
                //set the output voltage of the dcdc to max
                solar.dcdc.duty = 0;
                //enable dcdc
                solar.dcdc.enable = 1;
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

        //MPPT operation
        //increasig duty cycle -> DCDC output voltage is trying to decrease until the potentiometer set limig (~ 10.5V)
        //decreasing duty cycle -> DCDC output voltage is trying to increase up to the set limit (~ 14.4V)
            
        //check if solar panel is drawing current from the battery
        if (solar.adc.solar_current < 0)
        {
            solar.dcdc.duty -= SOLAR_MPPT_DUTY_STEP_BIG;
            //if duty cycle is at min and current is still negative sun must have been gone
            if (solar.dcdc.duty <= 0)
            {
                //stop solar panel draning the battery
                solar.dcdc.enable = 0;
                //not allowed to change enable state for the deadtime
                solar.mppt.deadtime = SOLAR_MPPT_DEADTIME;
            }
        }
        else
        {
            if(solar.dcdc.duty == 0 && solar_power < 2)
            {
                solar.dcdc.enable = 0;
                solar.mppt.deadtime = SOLAR_MPPT_DEADTIME;
            }
                
                
            if (solar_power > 20)
                solar.fan_enable = 1;

            float error = SOLAR_MPPT_VOLTAGE - solar.adc.solar_voltage;
            solar.dcdc.duty += error * 2;
        }
    }

    if (solar.dcdc.duty > SOLAR_DCDC_MAX_DUTY)
        solar.dcdc.duty = SOLAR_DCDC_MAX_DUTY;
    else if(solar.dcdc.duty < 0)
        solar.dcdc.duty = 0;

    if (!solar.mppt.enable)
        solar.dcdc.enable = 0;

    //solar.mppt.prev_solar_voltage = solar.adc.solar_voltage;
    //solar.mppt.prev_solar_current = solar.adc.solar_current;    
    solar.mppt.prev_solar_power = solar_power;
}