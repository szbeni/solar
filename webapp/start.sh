#!/bin/bash

DEVID=00:0D:18:3A:67:89

sudo rfcomm unbind /dev/rfcomm0
sudo timeout 5s bluetoothctl scan on
sudo bluetoothctl info $DEVID
sudo bluetoothctl pair $DEVID
sudo bluetoothctl trust $DEVID
sudo bluetoothctl connect $DEVID
sudo rfcomm bind /dev/rfcomm0 $DEVID
sudo chmod 777 /dev/rfcomm0

if [[ -f /home/pi/solar/webapp/solar_serial.py ]]; then
    cd /home/pi/solar/webapp/
else
    cd /home/beni/workspace/solar/webapp/
fi

sudo rfcomm unbind /dev/rfcomm0; sudo rfcomm bind /dev/rfcomm0 $DEVID; python3 solar_serial.py
