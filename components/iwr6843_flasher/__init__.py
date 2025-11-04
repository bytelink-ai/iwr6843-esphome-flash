import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["sensor", "text_sensor"]

iwr6843_flasher_ns = cg.esphome_ns.namespace("iwr6843_flasher")
IWR6843Flasher = iwr6843_flasher_ns.class_(
    "IWR6843Flasher", cg.Component, uart.UARTDevice
)

CONF_FIRMWARE_URL = "firmware_url"
CONF_RESET_PIN = "reset_pin"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(IWR6843Flasher),
            cv.Optional(CONF_FIRMWARE_URL): cv.url,
            cv.Optional(CONF_RESET_PIN): cv.int_,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    
    if CONF_FIRMWARE_URL in config:
        cg.add(var.set_firmware_url(config[CONF_FIRMWARE_URL]))
    
    if CONF_RESET_PIN in config:
        cg.add(var.set_reset_pin(config[CONF_RESET_PIN]))

