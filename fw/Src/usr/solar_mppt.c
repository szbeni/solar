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
                solar.mppt.mppt_voltage = 0.89 * solar.adc.solar_voltage;
                //set the output voltage of the dcdc to max
                solar.dcdc.duty = 0;
                //enable dcdc
                solar.dcdc.enable = 1;
                solar.mppt.deadtime=SOLAR_MPPT_DEADTIME;                

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
        /*if (solar.adc.solar_current < 0)
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
        else*/
        {

            if (solar.mppt.deadtime  == 0)
            {
                if (solar.adc.solar_voltage < SOLAR_PANEL_VOLTAGE_MIN && solar.adc.solar_current < 1)
                {
                    solar.mppt.deadtime = SOLAR_MPPT_DEADTIME;
                    solar.dcdc.enable = 0;
                }
            }
            else
            {
                solar.mppt.deadtime--;
            }

            if (solar.dcdc.duty == 0 && solar.adc.solar_voltage > 30)
            {
                solar_dcdc_enable(0);
                HAL_Delay(10);
                solar_dcdc_enable(1);
            }

            if (solar_power > 20)
                solar.fan_enable = 1;

            if(solar.dcdc.duty==0 && solar.adc.solar_current > 1)
            {
                solar.dcdc.duty = 0;
            }
            else
            {
                float error = solar.mppt.mppt_voltage - solar.adc.solar_voltage;
                solar.mppt.stateI += error;
                
                if (solar.mppt.stateI > solar.mppt.limitI)
                    solar.mppt.stateI = solar.mppt.limitI;
                else if (solar.mppt.stateI < -solar.mppt.limitI)  
                    solar.mppt.stateI = -solar.mppt.limitI;

                solar.dcdc.duty += error * solar.mppt.kP + solar.mppt.kI * solar.mppt.stateI;                
            }
        }
    }

    if (solar.dcdc.duty > SOLAR_DCDC_MAX_DUTY)
        solar.dcdc.duty = SOLAR_DCDC_MAX_DUTY;
    else if(solar.dcdc.duty < 0)
        solar.dcdc.duty = 0;


    solar.mppt.prev_solar_power = solar_power;
}