#include "solar.h"

void solar_adc_init(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)solar.adc.raw_values, 5);
}


//Solar panel values
static float solar_adc_get_solar_voltage(uint16_t raw)
{
    //voltage divider 20k, 1k8
    float scaled;
    scaled = (raw + CALIB_SOLAR_VOLTAGE_OFFSET) * ADS1115_SCALER * (20.0 + 1.8) / 1.8 * CALIB_SOLAR_VOLTAGE_GAIN;
    return scaled;
}

static float solar_adc_get_solar_current(uint16_t raw)
{
    //ACS712 30A version 2.5V + 0.066 Volt/Amps (bi directional)

    float scaled;
    //scaled = ((int16_t)raw - 16384 + solar.adc.ads1115_offset[2] ) * ADS1115_SCALER / 0.066 * CALIB_SOLAR_CURRENT_GAIN;
    scaled = ((int16_t)raw -16384 + solar.adc.ads1115_offset[2] ) * ADS1115_SCALER / 0.388885 * 1.8 * CALIB_SOLAR_CURRENT_GAIN;
    return scaled;
}


//Battery values
static float solar_adc_get_battery_voltage(uint16_t raw)
{
    //voltage divider 15k, 3k9

    float scaled;
    scaled = 3.3 / 4095.0 * (15.0 + 3.9) / 3.9 * (raw + CALIB_BATTERY_VOLTAGE_OFFSET) * CALIB_BATTERY_VOLTAGE_GAIN;
    return scaled;
}

static float solar_adc_get_battery_current(uint16_t raw)
{
    //ACS712 30A version 2.5V + 0.066 Volt/Amps (bi directional)
    float scaled;
    scaled = ((int16_t)raw - 16384 + solar.adc.ads1115_offset[3] ) * ADS1115_SCALER / 0.066 * CALIB_BATTERY_CURRENT_GAIN;
    return scaled;
}

//Load values
static float solar_adc_get_load_current(uint16_t raw)
{
    //ACS712 30A version 2.5V + 0.066 Volt/Amps (bi directional)
    float scaled;
    scaled = ((int16_t)raw - 16384 + solar.adc.ads1115_offset[1] ) * ADS1115_SCALER / 0.066 * CALIB_LOAD_CURRENT_GAIN;
    return scaled;
}


void solar_adc_get_values(void)
{
    //internal ADC
    solar.adc.battery_voltage = solar_adc_get_battery_voltage(solar.adc.raw_values[2]);
    solar.adc.load_voltage = solar.adc.battery_voltage;

    //external 16 bit ADS1115
    solar.adc.solar_voltage = solar_adc_get_solar_voltage(solar.adc.ads1115_values[0]);
    solar.adc.load_current = solar_adc_get_load_current(solar.adc.ads1115_values[1]);
    solar.adc.solar_current = solar_adc_get_solar_current(solar.adc.ads1115_values[2]);
    solar.adc.battery_current = solar_adc_get_battery_current(solar.adc.ads1115_values[3]);
    
}