#include "solar.h"


static void PI_settings_max_power(void)
{
    solar.mppt.kI = 0.1;
    solar.mppt.kP = 1.5;
    solar.mppt.limitI = 100;
    solar.mppt.stateI = 0;
}

static void PI_settings_float_charging(void)
{
    solar.mppt.kI = 0;
    solar.mppt.kP = 3;
    solar.mppt.limitI = 0;
    solar.mppt.stateI = 0;
}

static void solar_mppt_change_state(uint8_t state, uint16_t deadtime)
{
    if(solar.mppt.state != state)
    {
        solar.mppt.state = state;
        solar.mppt.deadtime = deadtime;

        //do things only once on entering state
        if(state == SOLAR_MPPT_STATE_OFF)
        {
            solar.dcdc.enable = 0;
        }
        else if(state == SOLAR_MPPT_STATE_MAX_POWER)
        {
            solar.dcdc.enable = 1;
            PI_settings_max_power();
        }
        else if (state == SOLAR_MPPT_STATE_FLOAT_CHARGING)
        {
            solar.dcdc.enable = 1;
            PI_settings_float_charging();
        }
    }
}

static float solar_mppt_PI_controller(float error)
{
    solar.mppt.stateI += error;

    if (solar.mppt.stateI > solar.mppt.limitI)
        solar.mppt.stateI = solar.mppt.limitI;
    else if (solar.mppt.stateI < -solar.mppt.limitI)  
        solar.mppt.stateI = -solar.mppt.limitI;

    return error * solar.mppt.kP + solar.mppt.kI * solar.mppt.stateI;
}

void solar_mppt_init(void)
{
    PI_settings_max_power();
}

void solar_mppt(void)
{
    //calculate power from solar
    float solar_power = solar.adc.solar_current * solar.adc.solar_voltage;
    
    uint8_t fan_speed = 0;
    //check if we need to enable fan
    if (solar_power > 0)
    {
        float speed = solar_power * 2.0;
        if (speed > 255)
            speed = 255;
        fan_speed = (uint8_t)(speed); 
    }
        
    solar_fan_set_speed(fan_speed);
   
    

    if (!solar.mppt.enable)
    {
        solar_mppt_change_state(SOLAR_MPPT_STATE_OFF, SOLAR_MPPT_DEADTIME_DEFAULT);
    }

    if(solar.mppt.deadtime > 0)
        solar.mppt.deadtime--;
    
    //when DCDC is off
    if (solar.mppt.state == SOLAR_MPPT_STATE_OFF)
    {

        //check if there is sun, solar panel voltage is above minimum level
        if (solar.adc.solar_voltage > SOLAR_PANEL_VOLTAGE_MIN && solar.mppt.deadtime == 0)
        {
            //set mppt voltage
            solar.mppt.mppt_voltage = 0.9 * solar.adc.solar_voltage;

            //change state
            solar_mppt_change_state(SOLAR_MPPT_STATE_MAX_POWER, SOLAR_MPPT_DEADTIME_DEFAULT);
            
        } 
    }
    else
    {
        //set mppt voltage back if it has crawled away
        if (solar.dcdc.duty == 0)
        {
            if (solar.adc.solar_voltage < solar.mppt.mppt_voltage)
                solar.mppt.mppt_voltage = 0.9* solar.adc.solar_voltage;
        }
        
        //check solar panel is sapping the battery..
        if(solar.mppt.deadtime == 0 && solar.adc.solar_voltage < solar.adc.battery_voltage)
        {
            solar.dcdc.duty -= SOLAR_MPPT_DUTY_STEP_BIG;
            //if duty cycle is at min and current is still negative sun must have been gone
            if (solar.dcdc.duty <= 0)
            {
                //stop solar panel draning the battery
                solar.dcdc.enable = 0;
                //change to off state
                solar_mppt_change_state(SOLAR_MPPT_STATE_OFF, SOLAR_MPPT_DEADTIME_DEFAULT);

            }
        }
       //check if solar panel is drawing current from the battery
       /*
       else if (solar.mppt.deadtime == 0 && (solar.adc.solar_current < 0.2 || solar_power < 1))
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
        }*/
        else
        {
            if (solar.mppt.state == SOLAR_MPPT_STATE_MAX_POWER)
            {
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

                    if(solar.mppt.mppt_voltage < SOLAR_PANEL_VOLTAGE_MIN)
                        solar.mppt.mppt_voltage = SOLAR_PANEL_VOLTAGE_MIN;
                    else if (solar.mppt.mppt_voltage > SOLAR_PANEL_VOLTAGE_MAX)
                        solar.mppt.mppt_voltage = SOLAR_PANEL_VOLTAGE_MAX;
                    
                    solar.mppt.power_average = power_average;
                }

                float output = solar_mppt_PI_controller(solar.mppt.mppt_voltage - solar.adc.solar_voltage);
                solar.dcdc.duty -= output;

                if (solar.mppt.deadtime == 0 && solar.adc.battery_voltage > SOLAR_MPPT_FLOAT_CHARGING_VOLTAGE_ENTER)
                {
                    solar_mppt_change_state(SOLAR_MPPT_STATE_FLOAT_CHARGING, 100);
                }
            }

            if (solar.mppt.state == SOLAR_MPPT_STATE_FLOAT_CHARGING)
            {
                float output = solar_mppt_PI_controller(SOLAR_MPPT_FLOAT_CHARGING_VOLTAGE_CONTROL - solar.adc.battery_voltage);
                solar.dcdc.duty += output;

                if (solar.adc.battery_voltage < SOLAR_MPPT_FLOAT_CHARGING_VOLTAGE_EXIT)
                {
                    if (solar.mppt.float_charge_exit_counter > 0)
                        solar.mppt.float_charge_exit_counter--;
                }
                else
                {
                    solar.mppt.float_charge_exit_counter = 1000;
                }

                if (solar.mppt.deadtime == 0 &&  solar.mppt.float_charge_exit_counter == 0)
                {
                    solar_mppt_change_state(SOLAR_MPPT_STATE_MAX_POWER, 100);
                }
            }                        
        }

    }

    if (solar.dcdc.duty > SOLAR_DCDC_MAX_DUTY)
        solar.dcdc.duty = SOLAR_DCDC_MAX_DUTY;
    else if(solar.dcdc.duty < 0)
        solar.dcdc.duty = 0;

    solar.mppt.prev_solar_power = solar_power;
}

