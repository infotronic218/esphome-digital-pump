import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome import components as cp
from esphome.components import sensor, text_sensor, number,switch,   gpio, i2c
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
    UNIT_CELSIUS,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT
)
UNIT_EC = 'ms/cm'
CONF_CHANNEL = "channel"
CONF_CALIBRATION_MODE = "calibration_mode"
CONF_KVALUE_HIGH = "kvalue_high"
CONF_KVALUE_LOW = "kvalue_low"
CONF_TEMPERATURE = "temperature"
CONF_PH_SENSOR = "ph_sensor"
CONF_ADS1115_ID = "id_ads1115"
MULTI_CONF = True

DEPENDENCIES = ['esp32']
AUTO_LOAD = ["number", "template", "sensor", "switch"]

dfrobot_ads1115_ns = cg.esphome_ns.namespace("dfrobot_ads1115_ec_")
DFRobotADS1115EC = dfrobot_ads1115_ns.class_("DFRobotADS1115EC", cg.PollingComponent)
DIGITAL_SWITCH = dfrobot_ads1115_ns.class_("DigitalSwitch", switch.Switch, cg.Component)

ads1115_adc_ns = cg.esphome_ns.namespace("ads1115_adc_")
ADS1115_ADC = ads1115_adc_ns.class_("ADS1115_ADC", cg.Component)

CONFIG_SCHEMA = cv.Schema({
      cv.GenerateID(): cv.declare_id(DFRobotADS1115EC),
      cv.GenerateID(CONF_ADS1115_ID): cv.use_id(ADS1115_ADC),
      cv.Required(CONF_CHANNEL):int,
      cv.Required(CONF_KVALUE_HIGH):cv.float_,
      cv.Required(CONF_KVALUE_LOW):cv.float_,
      cv.Optional(CONF_TEMPERATURE, default=25.0):cv.float_,
      cv.Required(CONF_PH_SENSOR):sensor.sensor_schema(
                unit_of_measurement=UNIT_EC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_EMPTY,
                state_class=STATE_CLASS_MEASUREMENT,
        ),

      cv.Required(CONF_CALIBRATION_MODE):switch.SWITCH_SCHEMA.extend({
            cv.GenerateID(): cv.declare_id(DIGITAL_SWITCH)
        }).extend(cv.COMPONENT_SCHEMA)
    }).extend(cv.polling_component_schema("5s"))
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

@coroutine_with_priority(45.0)
async def to_code(config):
    var =  cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    #await cg.register_component(var, config)
    ads1115 = await cg.get_variable(config[CONF_ADS1115_ID])
    cg.add(var.set_ads1115(ads1115))

    if CONF_PH_SENSOR in config:
        conf = config[CONF_PH_SENSOR]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ph_sensor(sens))

    if  CONF_CALIBRATION_MODE in config:
        conf = config[CONF_CALIBRATION_MODE]
        my_switch = await switch.new_switch(conf)
        cg.add(var.set_calibration_mode_switch(my_switch))

    if CONF_CHANNEL in config:
        channel = config[CONF_CHANNEL]
        cg.add(var.set_channel(channel))
    

    if CONF_TEMPERATURE in config:
        temp = config[CONF_TEMPERATURE]
        cg.add(var.set_temperature(temp))

    if CONF_KVALUE_HIGH in config:
        kv = config[CONF_KVALUE_HIGH]
        cg.add(var.set_cal_kvalue_high(kv))
    
    if CONF_KVALUE_LOW in config:
        kv = config[CONF_KVALUE_LOW]
        cg.add(var.set_cal_kvalue_low(kv))