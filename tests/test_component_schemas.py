"""Schema structure tests for atorch_dl24 ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import components.atorch_dl24 as hub  # noqa: E402
from components.atorch_dl24 import (  # noqa: E402
    binary_sensor,
    button,  # noqa: E402
    sensor,
    text_sensor,
)


class TestHubConstants:
    def test_conf_atorch_dl24_id_defined(self):
        assert hub.CONF_ATORCH_DL24_ID == "atorch_dl24_id"

    def test_conf_check_crc_defined(self):
        assert hub.CONF_CHECK_CRC == "check_crc"


class TestSensorDefs:
    def test_sensor_defs_completeness(self):
        from esphome.const import (
            CONF_CAPACITY,
            CONF_CURRENT,
            CONF_ENERGY,
            CONF_FREQUENCY,
            CONF_POWER,
            CONF_POWER_FACTOR,
            CONF_TEMPERATURE,
            CONF_VOLTAGE,
        )

        assert CONF_VOLTAGE in sensor.SENSOR_DEFS
        assert CONF_CURRENT in sensor.SENSOR_DEFS
        assert CONF_POWER in sensor.SENSOR_DEFS
        assert CONF_ENERGY in sensor.SENSOR_DEFS
        assert CONF_TEMPERATURE in sensor.SENSOR_DEFS
        assert CONF_CAPACITY in sensor.SENSOR_DEFS
        assert CONF_FREQUENCY in sensor.SENSOR_DEFS
        assert CONF_POWER_FACTOR in sensor.SENSOR_DEFS
        assert sensor.CONF_RUNTIME in sensor.SENSOR_DEFS
        assert len(sensor.SENSOR_DEFS) == 13


class TestBinarySensorDefs:
    def test_binary_sensor_defs_completeness(self):
        assert binary_sensor.CONF_RUNNING in binary_sensor.BINARY_SENSOR_DEFS
        assert len(binary_sensor.BINARY_SENSOR_DEFS) == 1


class TestTextSensors:
    def test_text_sensors_list(self):
        assert text_sensor.CONF_RUNTIME_FORMATTED in text_sensor.TEXT_SENSORS
        assert len(text_sensor.TEXT_SENSORS) == 1


class TestButtonConstants:
    def test_buttons_dict_completeness(self):
        assert button.CONF_RESET_ENERGY in button.BUTTONS
        assert button.CONF_RESET_CAPACITY in button.BUTTONS
        assert button.CONF_RESET_RUNTIME in button.BUTTONS
        assert button.CONF_RESET_ALL in button.BUTTONS
        assert button.CONF_PLUS in button.BUTTONS
        assert button.CONF_MINUS in button.BUTTONS
        assert button.CONF_SETUP in button.BUTTONS
        assert button.CONF_ENTER in button.BUTTONS
        assert button.CONF_USB_PLUS in button.BUTTONS
        assert button.CONF_USB_MINUS in button.BUTTONS
        assert len(button.BUTTONS) == 10

    def test_button_addresses_are_unique(self):
        addresses = list(button.BUTTONS.values())
        assert len(addresses) == len(set(addresses))
