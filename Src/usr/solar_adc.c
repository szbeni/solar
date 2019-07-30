#include "solar.h"
 
//Modify it by hand after measured everything
#define CALIB_SOLAR_VOLTAGE_OFFSET 64
#define CALIB_SOLAR_VOLTAGE_GAIN   1.0
#define CALIB_SOLAR_CURRENT_OFFSET 80
#define CALIB_SOLAR_CURRENT_GAIN   1.0

#define CALIB_BATTERY_VOLTAGE_OFFSET 157
#define CALIB_BATTERY_VOLTAGE_GAIN   1.0
#define CALIB_BATTERY_CURRENT_OFFSET 0
#define CALIB_BATTERY_CURRENT_GAIN   1.0

#define CALIB_LOAD_CURRENT_OFFSET 0
#define CALIB_LOAD_CURRENT_GAIN   1.0

void solar_adc_init(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)solar.adc.raw_values, 5);
}


//Solar panel values
static float solar_adc_get_solar_voltage(uint16_t raw)
{
    float scaled;
    scaled = 3.3 / 4095.0 * (20.0 + 1.8) / 1.8 * (raw + CALIB_SOLAR_VOLTAGE_OFFSET) * CALIB_SOLAR_VOLTAGE_GAIN;
    return scaled;
}

static float solar_adc_get_solar_current(uint16_t raw)
{
    //ACS712 30A version 2.5V + 0.066 Volt/Amps (bi directional)
    //scaled down to 3.3V range 2/3 divider

    float scaled;
    scaled = ((int16_t)raw - 2048 + CALIB_SOLAR_CURRENT_OFFSET ) * 3.3 / 4095.0 * (1.0 / 0.044) * CALIB_SOLAR_CURRENT_GAIN;
    return scaled;
}


//Battery values
static float solar_adc_get_battery_voltage(uint16_t raw)
{
    float scaled;
    scaled = 3.3 / 4095.0 * (15.0 + 3.9) / 3.9 * (raw + CALIB_BATTERY_VOLTAGE_OFFSET) * CALIB_BATTERY_VOLTAGE_GAIN;
    return scaled;
}

static float solar_adc_get_battery_current(uint16_t raw)
{
    //ACS712 30A version 2.5V + 0.066 Volt/Amps (bi directional)
    //scaled down to 3.3V range 2/3 divider

    float scaled;
    scaled = ((int16_t)raw - 2048 + CALIB_BATTERY_CURRENT_OFFSET ) * 3.3 / 4095.0 * (1.0 / 0.044) * CALIB_BATTERY_CURRENT_GAIN;
    return scaled;
}

//Load values
static float solar_adc_get_load_current(uint16_t raw)
{
    //ACS712 30A version 2.5V + 0.066 Volt/Amps (bi directional)
    //scaled down to 3.3V range 2/3 divider
    float scaled;
    scaled = ((int16_t)raw - 2048 + CALIB_LOAD_CURRENT_OFFSET ) * 3.3 / 4095.0 * (1.0 / 0.044)  * CALIB_LOAD_CURRENT_GAIN;
    return scaled;
}


void solar_adc_get_values(void)
{
    solar.adc.solar_voltage = solar_adc_get_solar_voltage(solar.adc.raw_values[0]);
    solar.adc.solar_current = solar_adc_get_solar_current(solar.adc.raw_values[1]);
    solar.adc.battery_voltage = solar_adc_get_battery_voltage(solar.adc.raw_values[2]);
    solar.adc.battery_current = solar_adc_get_battery_current(solar.adc.raw_values[3]);
    solar.adc.load_voltage = solar.adc.battery_voltage;
    solar.adc.load_current = solar_adc_get_load_current(solar.adc.raw_values[4]);
}