#!/bin/bash

sudo socat PTY,link=/dev/ttyS10 PTY,link=/dev/ttyS11 &
sudo chmod 777 /dev/ttyS10
sudo chmod 777 /dev/ttyS11

while true
do
    counter=1
    while [ $counter -le 100 ]
    do
        echo "$counter,2,3,4,5,6,7,8" > /dev/ttyS11
	((counter++))
	sleep 0.05
    done
done
