# Mppt Solar Charge Controller

An MPPT solar charge controller for a caravan.
Hardware, Firmware, Webapp for visualization

## Hardware main parts:
 - STM32 MCU (bluepill STM32F103)
 - SZBK07 DCDC module (LM25116 DCDC driver chip and other electroics removed just power electronic parts used)
 - 250W solar panel 36V nominal voltage
 - Split charge relay for charging from engine, when driving (separate from this circuit)
 - 55Ah and 110Ah 12V leisure batteries in parallel
 - 3x ACS712 current sensors
 - ADS1115 ADC for current measurement
 - FAN for cooling power electronics
 - Bluetooth UART module for communnication

<img src="https://github.com/szbeni/solar/raw/master/res/prototype01.jpg" width="400">


## Firmware:
- Written in C
- STCubeMX generated base project + user code
- 50Hz controlling loop
- DCDC half bridge controlling via complementary PWM signals
- MPPT tracking, float charging
- simple ASCII UART protocol for communication
- active PWM fan control when power is high

## Webapp:
- Python3 based
- Flexx UI
- For visualization and active control of the firmware
- Connection via bluetooth
- Could be running on a raspberry PI

![alt text](https://github.com/szbeni/solar/raw/master/res/solar_webapp.png "Webapp")


TODO:
Android app with same features as for webapp

