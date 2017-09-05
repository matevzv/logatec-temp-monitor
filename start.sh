#!/bin/bash

cd /root/logatec-temp-monitor/firmware
make temp-monitor.load

cd /root/logatec-temp-monitor
python measure.py
