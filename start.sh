#!/bin/bash

cd /root/logatec-temp-monitor/firmware
make temp-monitor.loadbone

cd /root/logatec-temp-monitor
python measure.py
