import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome import components as cp
from esphome.components import sensor, text_sensor, number,switch,   gpio, i2c
from esphome.core import CORE, coroutine_with_priority
from esphome import automation
from esphome.const import (
    CONF_ID,
)
CONF_ADDRESS = "address"
CONF_SDA = "sda"
CONF_SCL = "scl"
MULTI_CONF = True


DEPENDENCIES = ['esp32']
AUTO_LOAD = ["number", "template", "sensor", "switch"]

ads1115_adc_ns = cg.esphome_ns.namespace("ads1115_adc_")
ADS1115_ADC = ads1115_adc_ns.class_("ADS1115_ADC", cg.Component)

CONFIG_SCHEMA = cv.Schema({
      cv.GenerateID(): cv.declare_id(ADS1115_ADC),
      cv.Required(CONF_ADDRESS):int,
      cv.Required(CONF_SDA):int,
      cv.Required(CONF_SCL):int,
    }).extend(cv.COMPONENT_SCHEMA)
    #.extend(i2c.i2c_device_schema(CONF_I2C_ADDR))



#@coroutine_with_priority(45.0)
async def to_code(config):
    var =  cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    #await cg.register_component(var, config)

    if CONF_ADDRESS in config:
        addr = config[CONF_ADDRESS]
        cg.add(var.set_address(addr))
    
    if CONF_SDA in config:
        sda = config[CONF_SDA]
        cg.add(var.set_sda_pin(sda))

    if CONF_SCL in config:
        scl = config[CONF_SCL]
        cg.add(var.set_scl_pin(scl))
