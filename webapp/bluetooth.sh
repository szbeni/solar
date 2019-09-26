#!/bin/bash

sudo rfcomm bind /dev/rfcomm0 00:0D:18:3A:67:89
sudo chmod 777 /dev/rfcomm0
minicom -D /dev/rfcomm0
