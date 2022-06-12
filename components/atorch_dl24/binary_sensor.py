import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_ENTITY_CATEGORY,
    CONF_ICON,
    CONF_ID,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

from . import ATORCH_DL24_COMPONENT_SCHEMA, CONF_ATORCH_DL24_ID

DEPENDENCIES = ["atorch_dl24"]

CODEOWNERS = ["@syssi"]

CONF_RUNNING = "running"

ICON_RUNNING = "mdi:power"

BINARY_SENSORS = [
    CONF_RUNNING,
]

CONFIG_SCHEMA = ATORCH_DL24_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_RUNNING): binary_sensor.BINARY_SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(binary_sensor.BinarySensor),
                cv.Optional(CONF_ICON, default=ICON_RUNNING): cv.icon,
                cv.Optional(
                    CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_DIAGNOSTIC
                ): cv.entity_category,
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_ATORCH_DL24_ID])
    for key in BINARY_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await binary_sensor.register_binary_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
