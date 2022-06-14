import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_ID

from .. import ATORCH_DL24_COMPONENT_SCHEMA, CONF_ATORCH_DL24_ID, atorch_dl24_ns

DEPENDENCIES = ["atorch_dl24"]

CODEOWNERS = ["@syssi"]

# FF.55.11.01.01.00.00.00.00.57 reset_energy
# FF.55.11.01.02.00.00.00.00.50 reset_capacity
# FF.55.11.01.03.00.00.00.00.51 reset_runtime
# FF.55.11.01.05.00.00.00.00.53 reset_all
# FF.55.11.01.11.00.00.00.00.67 plus
# FF.55.11.01.12.00.00.00.00.60 minus
# FF.55.11.01.31.00.00.00.00.07 setup
# FF.55.11.01.32.00.00.00.00.00 enter
# FF.55.11.03.33.00.00.00.00.03 usb_plus
# FF.55.11.03.34.00.00.00.00.0c usb_minus

# FF.55.11.01.21.00.00.00.01.70 dim_backlight(0...60)
# FF.55.11.01.22.00.00.00.01.71 set_price(1...999999)

CONF_RESET_ENERGY = "reset_energy"
CONF_RESET_CAPACITY = "reset_capacity"
CONF_RESET_RUNTIME = "reset_runtime"
CONF_RESET_ALL = "reset_all"
CONF_PLUS = "plus"
CONF_MINUS = "minus"
CONF_SETUP = "setup"
CONF_ENTER = "enter"
CONF_USB_PLUS = "usb_plus"
CONF_USB_MINUS = "usb_minus"

ICON_RESET = "mdi:history"
ICON_PLUS = "mdi:plus-circle-outline"
ICON_MINUS = "mdi:minus-circle-outline"
ICON_SETUP = "mdi:cog"
ICON_ENTER = "mdi:keyboard-return"
ICON_USB_PLUS = "mdi:plus-circle-outline"
ICON_USB_MINUS = "mdi:minus-circle-outline"

BUTTONS = {
    CONF_RESET_ENERGY: 0x01,
    CONF_RESET_CAPACITY: 0x02,
    CONF_RESET_RUNTIME: 0x03,
    CONF_RESET_ALL: 0x05,
    CONF_PLUS: 0x11,
    CONF_MINUS: 0x12,
    CONF_SETUP: 0x31,
    CONF_ENTER: 0x32,
    CONF_USB_PLUS: 0x33,
    CONF_USB_MINUS: 0x34,
}

AtorchButton = atorch_dl24_ns.class_("AtorchButton", button.Button, cg.Component)

CONFIG_SCHEMA = ATORCH_DL24_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_RESET_ENERGY): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_RESET): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RESET_CAPACITY): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_RESET): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RESET_RUNTIME): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_RESET): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RESET_ALL): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_RESET): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_PLUS): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_PLUS): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_MINUS): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_MINUS): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_ENTER): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_ENTER): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_SETUP): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_SETUP): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_USB_PLUS): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_USB_PLUS): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_USB_MINUS): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AtorchButton),
                cv.Optional(CONF_ICON, default=ICON_USB_MINUS): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_ATORCH_DL24_ID])
    for key, address in BUTTONS.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await button.register_button(var, conf)
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))
