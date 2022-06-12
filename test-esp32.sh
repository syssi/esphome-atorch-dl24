#!/bin/bash

esphome -s external_components_source components -s dl24_mac_address 41:B8:12:0A:A8:37 ${1:-run} ${2:-esp32-usb-meter-example.yaml}
