#include <stdint.h>
#include "main.h"

//LSB - 0.000125V  = FSR / 32768
#define ADS1115_SCALER 4.096 / 32768.0

#define SOLAR_DCDC_MAX_DUTY 5000                      //maximum duty cycle amount, set in CubeMX timer settings
#define SOLAR_PANEL_VOLTAGE_MAX 40.0              //nominal solar panel voltage
#define SOLAR_PANEL_VOLTAGE_MIN 20.0                  //switch on charging when solar voltage is greater than this
#define SOLAR_BATTERY_MIN_VOLTAGE 11.0                //switch off load when the battery voltage is less than this
#define SOLAR_BATTERY_VOLTAGE_SWITCH_ON_LOAD 12.1     //switch on the load when battery voltage is greater than this
#define SOLAR_MPPT_DUTY_STEP 1
#define SOLAR_MPPT_DUTY_STEP_BIG 30
#define SOLAR_MPPT_DEADTIME_OFF 100                      //not allowed to change enable state for the deadtime
#define SOLAR_MPPT_DEADTIME_ON 1000                      //not allowed to change to disabled state for the deadtime

//Modify it by hand after measured everything
#define CALIB_SOLAR_VOLTAGE_OFFSET 195
#define CALIB_SOLAR_VOLTAGE_GAIN   1.0

#define CALIB_BATTERY_VOLTAGE_OFFSET 157
#define CALIB_BATTERY_VOLTAGE_GAIN   1.0

#define CALIB_SOLAR_CURRENT_GAIN   1.0

#define CALIB_BATTERY_CURRENT_GAIN   1.0

#define CALIB_LOAD_CURRENT_GAIN   1.0
    


typedef enum { COMMAND_DCDC_ENABLE = 1, COMMAND_PWM_DOWN, COMMAND_PWM_UP, COMMAND_LOAD_ENABLE, COMMAND_FAN_ENABLE, COMMAND_MPPT_ENABLE} SOLAR_COMMAND;
typedef enum { SOLAR_MPPT_DIR_DOWN = 0, SOLAR_MPPT_DIR_UP = 1} SOLAR_MPPT_DIR;
typedef struct 
{
    //float prev_solar_current;
    //float prev_solar_voltage;
    uint8_t enable;
    float prev_solar_power;
    float power_sum;
    float power_average;
    
    uint8_t counter;
    uint8_t direction;
    uint16_t deadtime;
    float solar_voltage_oc;
    float mppt_scaler;
    float mppt_voltage;
    float kP;
    float kI;
    float stateI;
    float limitI;

} solar_mppt_struct;

typedef struct 
{
    uint16_t raw_values[5];
    uint16_t ads1115_values[4];
    int16_t ads1115_offset[4];
    float solar_voltage;
    float solar_current;
    float battery_voltage;
    float battery_current;
    float load_voltage;
    float load_current;
} solar_adc_struct;

typedef struct 
{
   uint8_t enable;
   int16_t duty;


} solar_dcdc_struct;

typedef struct 
{
    uint8_t tick;
    uint8_t error;
    uint16_t counter;
    solar_adc_struct adc;
    solar_dcdc_struct dcdc;
    solar_mppt_struct mppt;
    uint8_t load_enable;
    uint16_t load_enable_deadtime;
    uint8_t load_enable_user;
    uint8_t fan_enable;

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
void solar_dcdc_set_duty(int16_t duty);
void solar_dcdc_enable(uint8_t enable);
uint8_t solar_dcdc_is_enabled(void);

void solar_load_enable(uint8_t enable);
void solar_fan_enable(uint8_t enable);

void solar_comm_init(void);
uint8_t solar_comm_receive(void);

void solar_ads1115_read(void);
void solar_ads1115_reset_offsets(void);

void solar_mppt(void);
void solar_mppt_init(void);
