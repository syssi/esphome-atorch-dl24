import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_CAPACITY,
    CONF_CURRENT,
    CONF_ENERGY,
    CONF_FREQUENCY,
    CONF_POWER,
    CONF_POWER_FACTOR,
    CONF_TEMPERATURE,
    CONF_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_POWER_FACTOR,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ICON_COUNTER,
    ICON_CURRENT_AC,
    ICON_EMPTY,
    ICON_TIMELAPSE,
    ICON_TIMER,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_EMPTY,
    UNIT_HERTZ,
    UNIT_SECOND,
    UNIT_VOLT,
    UNIT_WATT,
    UNIT_WATT_HOURS,
)

from . import ATORCH_DL24_COMPONENT_SCHEMA, CONF_ATORCH_DL24_ID

DEPENDENCIES = ["atorch_dl24"]

CODEOWNERS = ["@syssi"]

CONF_DIM_BACKLIGHT = "dim_backlight"

CONF_USB_DATA_MINUS = "usb_data_minus"
CONF_USB_DATA_PLUS = "usb_data_plus"

CONF_PRICE_PER_KWH = "price_per_kwh"
CONF_RUNTIME = "runtime"

UNIT_AMPERE_HOURS = "Ah"

SENSOR_DEFS = {
    CONF_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 4,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CAPACITY: {
        "unit_of_measurement": UNIT_AMPERE_HOURS,
        "icon": "mdi:battery-medium",
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_ENERGY: {
        "unit_of_measurement": UNIT_WATT_HOURS,
        "icon": ICON_COUNTER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_TEMPERATURE: {
        "unit_of_measurement": UNIT_CELSIUS,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DIM_BACKLIGHT: {
        "unit_of_measurement": UNIT_SECOND,
        "icon": ICON_TIMER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_USB_DATA_MINUS: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_USB_DATA_PLUS: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_PRICE_PER_KWH: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_COUNTER,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_FREQUENCY: {
        "unit_of_measurement": UNIT_HERTZ,
        "icon": ICON_CURRENT_AC,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_POWER_FACTOR: {
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_POWER_FACTOR,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_RUNTIME: {
        "unit_of_measurement": UNIT_SECOND,
        "icon": ICON_TIMELAPSE,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
}

CONFIG_SCHEMA = ATORCH_DL24_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): sensor.sensor_schema(**kwargs)
        for key, kwargs in SENSOR_DEFS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_ATORCH_DL24_ID])
    for key in SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
