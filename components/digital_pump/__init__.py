import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome import components as cp
from esphome.components import sensor, text_sensor, number,switch,   gpio, i2c
from esphome.components import web_server_base
from esphome.components.web_server_base import CONF_WEB_SERVER_BASE_ID
from esphome.core import CORE, coroutine_with_priority
from esphome import automation
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_VOLUME,
    CONF_INITIAL_VALUE,
    CONF_LAMBDA,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_OPTIMISTIC,
    CONF_MODE,
    CONF_RESTORE_VALUE,
    CONF_INITIAL_VALUE,
    CONF_STEP,
)
CONF_SET_ACTION = "set_action"
CONF_PIN = "pin"
CONF_SPEED = "speed"
CONF_CLOCKWISE_ROTATION = "clockwise_rotation"
CONF_DOSE = "dose"
CONF_DOSER_ON = "doser_on"
CONF_AUTO_MODE = "auto_mode"
CONF_CALIBRATION = "calibration"
MULTI_CONF = True
print(cp)

DEPENDENCIES = ['esp32']
AUTO_LOAD = ["number", "template", "gpio", "switch"]

digital_pump_ns = cg.esphome_ns.namespace("digital_pump")
DIGITAL_PUMP = digital_pump_ns.class_("DigitalPump", cg.Component)


DIGITAL_SWITCH = digital_pump_ns.class_("DigitalSwitch", switch.Switch, cg.Component)


template_number_ns = cg.esphome_ns.namespace("digital_pump")
TemplateNumber = template_number_ns.class_("TemplateNumber",number.Number, cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema({
      cv.GenerateID(): cv.declare_id(DIGITAL_PUMP),
      cv.Required(CONF_PIN):int,
      cv.Required(CONF_CLOCKWISE_ROTATION):cv.boolean,
      cv.Required(CONF_SPEED):cv.float_,
      cv.Required(CONF_CALIBRATION):number.NUMBER_SCHEMA.extend({
            cv.GenerateID(): cv.declare_id(TemplateNumber),
            cv.Optional(CONF_MAX_VALUE, default=10000): cv.float_,
            cv.Optional(CONF_MIN_VALUE, default=0): cv.float_,
            cv.Optional(CONF_STEP, default=0.1): cv.positive_float,
            cv.Optional(CONF_LAMBDA): cv.returning_lambda,
            cv.Optional(CONF_OPTIMISTIC, default=True): cv.boolean,
            cv.Optional(CONF_SET_ACTION): automation.validate_automation(single=True),
            cv.Optional(CONF_INITIAL_VALUE): cv.float_,
            cv.Optional(CONF_RESTORE_VALUE): cv.boolean,
      }).extend(cv.polling_component_schema("60s")),
      cv.Required(CONF_DOSE):    number.NUMBER_SCHEMA.extend({
            cv.GenerateID(): cv.declare_id(TemplateNumber),
            cv.Optional(CONF_MAX_VALUE, default=10000): cv.float_,
            cv.Optional(CONF_MIN_VALUE, default=0): cv.float_,
            cv.Optional(CONF_STEP, default=0.1): cv.positive_float,
            cv.Optional(CONF_LAMBDA): cv.returning_lambda,
            cv.Optional(CONF_OPTIMISTIC, default=True): cv.boolean,
            #cv.Optional(CONF_MODE, default="box"): str,
            cv.Optional(CONF_SET_ACTION): automation.validate_automation(single=True),
            cv.Optional(CONF_INITIAL_VALUE): cv.float_,
            cv.Optional(CONF_RESTORE_VALUE): cv.boolean,
        }),

      cv.Required(CONF_DOSER_ON):switch.SWITCH_SCHEMA.extend({
            cv.GenerateID(): cv.declare_id(DIGITAL_SWITCH)
        }).extend(cv.COMPONENT_SCHEMA),

       cv.Required(CONF_AUTO_MODE):switch.SWITCH_SCHEMA.extend({
            cv.GenerateID(): cv.declare_id(DIGITAL_SWITCH)
        }).extend(cv.COMPONENT_SCHEMA),
    })
    #.extend(i2c.i2c_device_schema(CONF_I2C_ADDR))


async def add_update(var,config):
    if CONF_LAMBDA in config:
        template_ = await cg.process_lambda(
            config[CONF_LAMBDA], [], return_type=cg.optional.template(float)
        )
        cg.add(var.set_template(template_))

    else:
        cg.add(var.set_optimistic(config[CONF_OPTIMISTIC]))
        #cg.add(var.set_initial_value(config[CONF_INITIAL_VALUE]))
        if CONF_RESTORE_VALUE in config:
            cg.add(var.set_restore_value(config[CONF_RESTORE_VALUE]))

    if CONF_SET_ACTION in config:
        await automation.build_automation(
            var.get_set_trigger(), [(float, "x")], config[CONF_SET_ACTION]
        )

#@coroutine_with_priority(45.0)
async def to_code(config):
    var =  cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    #await cg.register_component(var, config)
    
    if CONF_DOSE in config:
        conf = config[CONF_DOSE]
        my_number = await number.new_number(conf, min_value=0, max_value=1000, step=conf[CONF_STEP])
        cg.add(var.set_dose_number(my_number))
        await add_update(my_number,conf)
        

    if CONF_CALIBRATION in config:
        conf = config[CONF_CALIBRATION]
        my_number = await number.new_number(conf, min_value=0, max_value=1000, step=conf[CONF_STEP])
        cg.add(var.set_calibration_number(my_number))
        await add_update(my_number,conf)

    if  CONF_DOSER_ON in config:
        conf = config[CONF_DOSER_ON]
        my_switch = await switch.new_switch(conf)
        cg.add(var.set_dose_on_switch(my_switch))

    if  CONF_AUTO_MODE in config:
        conf = config[CONF_AUTO_MODE]
        my_switch = await switch.new_switch(conf)
        cg.add(var.set_auto_mode_switch(my_switch))

    if CONF_PIN in config:
        pin = config[CONF_PIN]
        cg.add(var.set_pin(pin))

    if CONF_SPEED in config:
        speed = config[CONF_SPEED]
        cg.add(var.set_speed(speed))
    
    if CONF_CLOCKWISE_ROTATION in config:
        rotation = config[CONF_CLOCKWISE_ROTATION]
        cg.add(var.set_clockwise_rotation(rotation))