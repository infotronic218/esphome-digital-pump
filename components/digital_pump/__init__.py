import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor, text_sensor, i2c
from esphome.components import web_server_base
from esphome.components.web_server_base import CONF_WEB_SERVER_BASE_ID
from esphome.core import CORE, coroutine_with_priority
from esphome.const import (
    CONF_ID,
)

CONF_PIN = "pin"
CONF_DOSE = "dose"
CONF_CALIBRATION = "calibration"

MULTI_CONF = True


DEPENDENCIES = ['esp32']

digital_pump_ns = cg.esphome_ns.namespace("digital_pump")
DIGITAL_PUMP = digital_pump_ns.class_("DigitalPump", cg.Component)
CONFIG_SCHEMA = (
    cv.Schema({
      cv.GenerateID(): cv.declare_id(DIGITAL_PUMP),
      cv.Required(CONF_PIN):int,
      cv.Required(CONF_DOSE): float,
      cv.Required(CONF_CALIBRATION):float 
    })
    #.extend(i2c.i2c_device_schema(CONF_I2C_ADDR))
)



@coroutine_with_priority(45.0)
async def to_code(config):
    var =  cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    #cg.add(var.set_frequency(CONF_FREQUENCY))
    #cg.add(var.set_sda(CONF_SDA))
    #cg.add(var.set_scl(CONF_SCL))
        
    if CONF_PIN in config:
        pin = config[CONF_PIN]
        cg.add(var.set_pin(pin))
    if CONF_DOSE in config:
        dose = config[CONF_DOSE]
        cg.add(var.set_dose(dose))
    if CONF_CALIBRATION in config:
        cal = config[CONF_CALIBRATION]
        cg.add(var.set_calibration(cal))
