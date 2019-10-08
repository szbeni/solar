#include "solar.h"

#define ADS1115_ADDRESS 0x48


extern I2C_HandleTypeDef hi2c2;

void solar_ads1115_reinit(void)
{
	HAL_I2C_DeInit(&hi2c2);
	//Deinit should reset GPIOs to default state, but
	//maybe need to pul GPIO to input to be sure
	//GPIO_InitTypeDef GPIO_InitStruct = {0};
	

	HAL_Delay(10);
	//CubeMX already set this up at startup
	/*
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = 100000;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	*/
	HAL_I2C_Init(&hi2c2);
}

void solar_ads1115_reset_offsets(void)
{
	if (solar_ads1115_read() == HAL_OK)
	{
		//only for the curret chanels
		for( uint8_t i = 1; i < 4; i++) 
			solar.adc.ads1115_offset[i] = 16384 - solar.adc.ads1115_values[i];
	}
}

uint8_t solar_ads1115_read(void)
{
    uint8_t ADSwrite[6];
    int16_t value;
	HAL_StatusTypeDef status;

    for( uint8_t i = 0; i < 4; i++) 
    {

			ADSwrite[0] = 0x01;
			//config register high
			// 15    OS    - 0: No effect, 1: Start a single conversion (when in power-down state)
			// 14:12 MUX   - 000: AINP = AIN0 and AINN = AIN1 (default), 001: AINP = AIN0 and AINN = AIN3, 010: AINP = AIN1 and AINN = AIN3, 011: AINP = AIN2 and AINN = AIN3, 100: AINP = AIN0 and AINN = GND, 101: AINP = AIN1 and AINN = GND, 110: AINP = AIN2 and AINN = GND, 111: AINP = AIN3 and AINN = GND,
			// 11:9  PGA   - 000: FSR = ±6.144 V, 001: FSR = ±4.096 V, 010: FSR = ±2.048 V (default), 011: FSR = ±1.024 V, 100: FSR = ±0.512 V, 101: FSR = ±0.256 V, 110: FSR = ±0.256 V, 111: FSR = ±0.256 V
			// 8     MODE  - 0: Continuous-conversion mode, 1: Single-shot mode or power-down state (default)

			switch(i) {
				case(0):
					ADSwrite[1] = 0xC3; //11000011
				break;
				case(1):
					ADSwrite[1] = 0xD3; //11010011
				break;
				case(2):
					ADSwrite[1] = 0xE3;
				break;
				case(3):
					ADSwrite[1] = 0xF3;
				break;
			}

			//config register low
			// 7:5  DR        - 000: 8 SPS,001: 16 SPS,010: 32 SPS,011: 64 SPS,100: 128 SPS (default),101: 250 SPS,110: 475 SPS,111: 860 SPS
			// 4    COMP_MODE - 0: Traditional comparator, 1: Windows Comparator
			// 3    COMP_POL  - 0: Active low, 1: Active High
			// 2 	COMP_LAT  - 0: Nonlatching comparator, 1: Latching comparator
			// 1:0  COMP_QUE  - 00: Assert after one conversion, 01: Assert after two conversions, 10: Assert after four conversions, 11: Disable comparator and set ALERT/RDY pin to high-impedance (default)

			ADSwrite[2] = 0xE3; // 11100011

            //write config
			status = HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, ADSwrite, 3, 100);
			if (status != HAL_OK)
			{
				return status;
			}

            //select conv register
			ADSwrite[0] = 0x00;
			status = HAL_I2C_Master_Transmit(&hi2c2, ADS1115_ADDRESS<<1, ADSwrite, 1, 100);
			if (status != HAL_OK)
			{
				return status;
			}

			//wait for conv
            HAL_Delay(2);
            //read conv register
			status = HAL_I2C_Master_Receive(&hi2c2, ADS1115_ADDRESS<<1, ADSwrite, 2, 100);
			if (status != HAL_OK)
			{
				return status;
			}

			value = (ADSwrite[0] << 8 | ADSwrite[1]);
			if(value < 0 ) {
				value = 0;
			}
            solar.adc.ads1115_values[i] = value;
    }

	return status;
}