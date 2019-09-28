#include <stdint.h>
#include "main.h"

//LSB - 0.000125V  = FSR / 32768
#define ADS1115_SCALER 4.096 / 32768.0

#define SOLAR_DCDC_MAX_DUTY 5000                        //maximum duty cycle amount, set in CubeMX timer settings
#define SOLAR_PANEL_VOLTAGE_MAX 40.0                    //nominal solar panel voltage
#define SOLAR_PANEL_VOLTAGE_MIN 20.0                    //switch on charging when solar voltage is greater than this

#define SOLAR_BATTERY_LOAD_SWITCH_OFF_VOLTAGE 11.0      //switch off load when the battery voltage is less than this
#define SOLAR_BATTERY_LOAD_SWITCH_ON_VOLTAGE  12.5      //switch on the load when battery voltage is greater than this
#define SOLAR_BATTERY_LOAD_SWITCH_DEADTIME   5000       //do not switch load rapidly on and off

#define SOLAR_MPPT_DUTY_STEP 1
#define SOLAR_MPPT_DUTY_STEP_BIG 30

#define SOLAR_MPPT_DEADTIME_DEFAULT 1000                //

#define SOLAR_MPPT_FAN_ENABLE_POWER 35                  //when solar power is more that 35W enable the fan
#define SOLAR_MPPT_FAN_ENABLE_DEADTIME   1000           //deadtime

#define SOLAR_MPPT_FLOAT_CHARGING_VOLTAGE_ENTER   14.2    //when to enter to float charging mode from max power
#define SOLAR_MPPT_FLOAT_CHARGING_VOLTAGE_CONTROL 13.8    //what is the float charging voltage (demand for the controller)
#define SOLAR_MPPT_FLOAT_CHARGING_VOLTAGE_EXIT    13.2    //when to leave float charging mode and go back to max power mode

//Modify it by hand after measured everything
#define CALIB_SOLAR_VOLTAGE_OFFSET 195
#define CALIB_SOLAR_VOLTAGE_GAIN   1.0

#define CALIB_BATTERY_VOLTAGE_OFFSET 157
#define CALIB_BATTERY_VOLTAGE_GAIN   1.0

#define CALIB_SOLAR_CURRENT_GAIN   1.0

#define CALIB_BATTERY_CURRENT_GAIN   1.0

#define CALIB_LOAD_CURRENT_GAIN   1.0
    
//Comms
#define SOLAR_SEND_BUFFER_SIZE  128


typedef enum { COMMAND_DCDC_ENABLE = 1, COMMAND_PWM_DOWN, COMMAND_PWM_UP, COMMAND_LOAD_ENABLE, COMMAND_FAN_ENABLE, COMMAND_MPPT_ENABLE} SOLAR_COMMAND;
typedef enum { SOLAR_MPPT_DIR_DOWN = 0, SOLAR_MPPT_DIR_UP = 1} SOLAR_MPPT_DIR;
typedef enum { SOLAR_MPPT_STATE_OFF = 0, SOLAR_MPPT_STATE_MAX_POWER, SOLAR_MPPT_STATE_FLOAT_CHARGING} SOLAR_MPPT_STATE;
typedef struct 
{
    uint8_t state;
    uint16_t deadtime;

    uint8_t enable;
    float prev_solar_power;
    float power_sum;
    float power_average;
    
    uint8_t counter;
    uint8_t direction;
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
    uint8_t fan_speed;
    uint8_t send_buffer[SOLAR_SEND_BUFFER_SIZE];
} solar_struct;




extern solar_struct solar;
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

void solar_main(void);
void solar_50Hz_callback(void);

void solar_adc_init(void);
void solar_adc_get_values(void);

void solar_dcdc_init(void);
void solar_dcdc_set_duty(int16_t duty);
void solar_dcdc_enable(uint8_t enable);
uint8_t solar_dcdc_is_enabled(void);

void solar_load_enable(uint8_t enable);

void solar_fan_init(void);
void solar_fan_set_speed(uint8_t speed);

void solar_comm_init(void);
uint8_t solar_comm_receive(void);
void solar_comm_command_handler(uint8_t command);
void solar_comm_send_values(void);




void solar_ads1115_read(void);
void solar_ads1115_reset_offsets(void);

void solar_mppt(void);
void solar_mppt_init(void);
