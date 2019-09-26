#!/bin/bash

sudo rfcomm bind /dev/rfcomm0 00:0D:18:3A:67:89
sudo minicom -D /dev/rfcomm0
