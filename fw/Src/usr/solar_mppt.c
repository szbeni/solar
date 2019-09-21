#include "solar.h"

#define SOLAR_PANEL_NOMINAL_VOLTAGE 33.0
#define SOLAR_PANEL_VOLTAGE_MIN 16.0            //switch on charging when solar voltage is greater than this
#define BATTERY_MIN_VOLTAGE 11.0                //switch off load when the battery voltage is less than this
#define BATTERY_VOLTAGE_SWITCH_ON_LOAD 12.1     //switch on the load when battery voltage is greater than this
#define SOLAR_MPPT_DUTY_STEP 10



void solar_mppt(void)
{
    float solar_power = 0;
    //when DCDC is off
    if (!solar_dcdc_is_enabled())
    {
        //check if there is sun, solar panel voltage is above minimum level
        if (solar.adc.solar_voltage > SOLAR_PANEL_VOLTAGE_MIN)
        {
            //set the output voltage of the dcdc to max
            solar.dcdc.duty = 0;
            //enable dcdc
            solar.dcdc.enable = 1;
        }   
    }
    //when DCDC is on
    else
    {
        //check for negative current
        if (solar.adc.solar_current < 0)
        {
            solar.dcdc.duty -= 1;
            //if we are at max voltage and current is still negative something is wrong, switch off DCDC
            if (solar.dcdc.duty < 0)
            {
                solar.dcdc.duty  = 0;
                solar.dcdc.enable = 0;
            }
        }
        //MPPT operation
        else
        {
            solar_power = solar.adc.solar_current * solar.adc.solar_voltage;
            
            //check if solar panel voltage is below minimum
            if (solar.adc.solar_voltage < SOLAR_PANEL_VOLTAGE_MIN)
            {
                solar.dcdc.duty += SOLAR_MPPT_DUTY_STEP;
                //if duty cycle is at max and still voltage is below minimin, sun must have been gone
                if (solar.dcdc.duty >= SOLAR_DCDC_MAX_DUTY)
                {
                    solar.dcdc.enable = 0;
                }
            }
            else
            {
                //MPPT
                switch(solar.mppt.direction)
                {
                    case SOLAR_MPPT_DIR_RIGHT :
                        if (solar_power > solar.mppt.prev_solar_power){
                            solar.dcdc.duty -= SOLAR_MPPT_DUTY_STEP;
                        }
                        else
                        {
                            solar.dcdc.duty -= SOLAR_MPPT_DUTY_STEP;
                            solar.mppt.direction = SOLAR_MPPT_DIR_LEFT;
                        }
                        break;
                    case SOLAR_MPPT_DIR_LEFT :
                        if (solar_power >= solar.mppt.prev_solar_power){
                            solar.dcdc.duty += SOLAR_MPPT_DUTY_STEP;
                        }
                        else
                        {
                            solar.dcdc.duty -= SOLAR_MPPT_DUTY_STEP;
                            solar.mppt.direction = SOLAR_MPPT_DIR_RIGHT;
                        }
                        
                }
            }
        }
    }

    if (solar.dcdc.duty > SOLAR_DCDC_MAX_DUTY)
        solar.dcdc.duty = SOLAR_DCDC_MAX_DUTY;
    else if(solar.dcdc.duty < 0)
        solar.dcdc.duty = 0;

    solar_dcdc_set_duty(solar.dcdc.duty);
    solar_dcdc_enable(solar.dcdc.enable);    

    //solar.mppt.prev_solar_voltage = solar.adc.solar_voltage;
    //solar.mppt.prev_solar_current = solar.adc.solar_current;    
    solar.mppt.prev_solar_power = solar_power;
}