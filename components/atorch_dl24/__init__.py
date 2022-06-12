import esphome.codegen as cg
from esphome.components import ble_client
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = ["binary_sensor", "button", "sensor", "text_sensor"]
CODEOWNERS = ["@syssi"]
MULTI_CONF = True

CONF_ATORCH_DL24_ID = "atorch_dl24_id"

CONF_CHECK_CRC = "check_crc"

atorch_dl24_ns = cg.esphome_ns.namespace("atorch_dl24")
AtorchDL24 = atorch_dl24_ns.class_("AtorchDL24", ble_client.BLEClientNode, cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(AtorchDL24),
        cv.Optional(CONF_CHECK_CRC, default=True): cv.boolean,
    }
).extend(ble_client.BLE_CLIENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)

    cg.add(var.set_check_crc(config[CONF_CHECK_CRC]))
