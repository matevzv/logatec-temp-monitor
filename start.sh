#!/bin/bash

MODEL=`cat /proc/device-tree/model | sed "s/ /_/g"`
case "$MODEL" in
	TI_AM335x_SNA-LGTC_*)
		PORT=/dev/ttyS2
		;;
	*)
		PORT=/dev/ttyUSB0
		;;
esac

cd /root/logatec-temp-monitor/firmware
make temp-monitor.load

cd /root/logatec-temp-monitor
python measure.py -d $PORT
