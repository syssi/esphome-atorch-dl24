import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import ICON_TIMELAPSE

from . import ATORCH_DL24_COMPONENT_SCHEMA, CONF_ATORCH_DL24_ID

DEPENDENCIES = ["atorch_dl24"]

CODEOWNERS = ["@syssi"]

CONF_RUNTIME_FORMATTED = "runtime_formatted"

TEXT_SENSORS = [
    CONF_RUNTIME_FORMATTED,
]

CONFIG_SCHEMA = ATORCH_DL24_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_RUNTIME_FORMATTED): text_sensor.text_sensor_schema(
            icon=ICON_TIMELAPSE
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_ATORCH_DL24_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = await text_sensor.new_text_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))
