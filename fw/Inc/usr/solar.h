#include <stdint.h>
#include "main.h"

typedef enum { COMMAND_DCDC_ENABLE = 1, COMMAND_PWM_DOWN, COMMAND_PWM_UP, COMMAND_LOAD_ENABLE } SOLAR_COMMAND;

typedef struct 
{
    uint16_t raw_values[5];
    float solar_voltage;
    float solar_current;
    float battery_voltage;
    float battery_current;
    float load_voltage;
    float load_current;
} solar_adc_struct;

typedef struct 
{
   uint8_t enable_user;
   uint8_t enable;
   uint16_t duty;


} solar_dcdc_struct;

typedef struct 
{
    uint8_t tick;
    uint8_t error;
    uint16_t counter;
    solar_adc_struct adc;
    solar_dcdc_struct dcdc;
    uint8_t load_enable;
} solar_struct;

extern solar_struct solar;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

void solar_main(void);
void solar_50Hz_callback(void);

void solar_adc_init(void);
void solar_adc_get_values(void);

void solar_dcdc_init(void);
void solar_dcdc_set_duty(uint16_t);
void solar_dcdc_enable(uint8_t enable);

void solar_load_enable(uint8_t enable);

void solar_comm_init(void);
uint8_t solar_comm_receive(void);