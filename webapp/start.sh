#!/bin/bash

sudo timeout 15s bluetoothctl scan on
sudo bluetoothctl info 00:0D:18:3A:67:89
sudo bluetoothctl pair 00:0D:18:3A:67:89
sudo bluetoothctl trust 00:0D:18:3A:67:89
sudo bluetoothctl connect 00:0D:18:3A:67:89
sudo rfcomm bind /dev/rfcomm0 00:0D:18:3A:67:89
sudo chmod 777 /dev/rfcomm0
python3 /home/pi/solar/webapp/solar_webapp.py