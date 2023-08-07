import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome import components as cp
from esphome.components import sensor, text_sensor, modbus_controller,  number,switch,   gpio, i2c
from esphome.components import web_server_base
from esphome.components.web_server_base import CONF_WEB_SERVER_BASE_ID
from esphome.core import CORE, coroutine_with_priority
from esphome.components.modbus_controller import *
from esphome import automation
from esphome.components.modbus_controller.sensor import ModbusSensor
from esphome.components.modbus_controller import MODBUS_REGISTER_TYPE, SENSOR_VALUE_TYPE
import esphome.helpers as myhelps
#print(myhelps)
#print(myhelps._TYPE_OVERLOADS[int](10) )
from esphome.const import (
    CONF_ID,
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
    STATE_CLASS_MEASUREMENT
)
CONFIG_PIN_RE = "pin_re"
CONFIG_PIN_DE =  "pin_de"
CONFIG_PIN_TX = "pin_tx"
CONFIG_PIN_RX = "pin_rx"
CONF_TEMPERATURE = "temperature"
CONF_HUMIDITY =  "humidity"
CONF_BULK_PERMITTIVITY = "bulk_permittivity"
CONF_PORE_WATER = "pore_water"
CONF_EC =  "ec"
CONF_WC = "wc"
MULTI_CONF = True
#print(cp)

DEPENDENCIES = ['esp32']
AUTO_LOAD = ["sensor", "modbus","modbus_controller"]
#print(modbus_controller)
tdr_soil_sensor__ns = cg.esphome_ns.namespace("tdr_soil_sensor_")
TDR_SOIL_SENSOR = tdr_soil_sensor__ns.class_("TDR_Soil_Sensor",  cg.Component)

#print(__file__)

TDR_TEMP_SENSOR = tdr_soil_sensor__ns.class_("TDR_Temp_Sensor",modbus_controller.sensor.ModbusSensor)
TDR_EC_SENSOR = tdr_soil_sensor__ns.class_("TDR_EC_Sensor",modbus_controller.sensor.ModbusSensor)
TDR_WC_SENSOR = tdr_soil_sensor__ns.class_("TDR_WC_Sensor",modbus_controller.sensor.ModbusSensor)

#print(modbus_controller.sensor.CONFIG_SCHEMA)
#template_number_ns = cg.esphome_ns.namespace("digital_pump")
#TemplateNumber = template_number_ns.class_("TemplateNumber",number.Number, cg.PollingComponent)

SENSOR_CONFIG_SCHEMA = cv.All(
    sensor.SENSOR_SCHEMA.extend(cv.COMPONENT_SCHEMA)
    .extend(ModbusItemBaseSchema)
    .extend(
        {
            cv.GenerateID(): cv.declare_id(ModbusSensor),
            cv.Optional(CONF_REGISTER_TYPE, default='read'): cv.enum(MODBUS_REGISTER_TYPE),
            cv.Optional(CONF_VALUE_TYPE, default="U_WORD"): cv.enum(SENSOR_VALUE_TYPE),
            cv.Optional(CONF_REGISTER_COUNT, default=0): cv.positive_int,
        }
    ),
    validate_modbus_register,
)



CONFIG_SCHEMA = cv.Schema({
      cv.GenerateID(): cv.declare_id(TDR_SOIL_SENSOR),
      cv.Optional(CONF_EC):SENSOR_CONFIG_SCHEMA,
      cv.Optional(CONF_TEMPERATURE):SENSOR_CONFIG_SCHEMA,
      cv.Optional(CONF_WC):SENSOR_CONFIG_SCHEMA
     # cv.Optional(CONF_TEMPERATURE):modbus_controller.sensor.CONFIG_SCHEMA
    }).extend(cv.polling_component_schema("60s"))


#@coroutine_with_priority(45.0)
async def to_code(config):
    var =  cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    #await cg.register_component(var, config)

    if CONF_WC in config:
        conf = config[CONF_WC]
        byte_offset, reg_count = modbus_calc_properties(conf)
        #print(byte_offset, reg_count)
        value_type = conf[CONF_VALUE_TYPE]
        register_type = conf[CONF_REGISTER_TYPE]
        register_type = MODBUS_REGISTER_TYPE['read']
        #print("Value Type",value_type)
        
        #print(type(value_type))
        #value_type = "U_WORD" 
        value_type = SENSOR_VALUE_TYPE['U_WORD']
        var = cg.new_Pvariable(
            conf[CONF_ID],
            register_type,
            conf[CONF_ADDRESS],
            byte_offset,
            conf[CONF_BITMASK],
            value_type,
            reg_count,
            conf[CONF_SKIP_UPDATES],
            conf[CONF_FORCE_NEW_RANGE],
        )
        await cg.register_component(var, conf)
        await sensor.register_sensor(var, conf)

        paren = await cg.get_variable(conf[CONF_MODBUS_CONTROLLER_ID])
        cg.add(paren.add_sensor_item(var))
        await add_modbus_base_properties(var, conf, TDR_WC_SENSOR)


    if CONF_EC in config:
        conf = config[CONF_EC]
        byte_offset, reg_count = modbus_calc_properties(conf)
        #print(byte_offset, reg_count)
        value_type = conf[CONF_VALUE_TYPE]
        register_type = conf[CONF_REGISTER_TYPE]
        register_type = MODBUS_REGISTER_TYPE['read']
        #print("Value Type",value_type)
        
        #print(type(value_type))
        #value_type = "U_WORD" 
        value_type = SENSOR_VALUE_TYPE['U_WORD']
        var = cg.new_Pvariable(
            conf[CONF_ID],
            register_type,
            conf[CONF_ADDRESS],
            byte_offset,
            conf[CONF_BITMASK],
            value_type,
            reg_count,
            conf[CONF_SKIP_UPDATES],
            conf[CONF_FORCE_NEW_RANGE],
        )
        await cg.register_component(var, conf)
        await sensor.register_sensor(var, conf)

        paren = await cg.get_variable(conf[CONF_MODBUS_CONTROLLER_ID])
        cg.add(paren.add_sensor_item(var))
        await add_modbus_base_properties(var, conf, TDR_EC_SENSOR)


    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        byte_offset, reg_count = modbus_calc_properties(conf)
        #print(byte_offset, reg_count)
        value_type = conf[CONF_VALUE_TYPE]
        register_type = conf[CONF_REGISTER_TYPE]
        register_type = MODBUS_REGISTER_TYPE['read']
        #print("Value Type",value_type)
        
        #print(type(value_type))
        #value_type = "U_WORD" 
        value_type = SENSOR_VALUE_TYPE['U_WORD']
        var = cg.new_Pvariable(
            conf[CONF_ID],
            register_type,
            conf[CONF_ADDRESS],
            byte_offset,
            conf[CONF_BITMASK],
            value_type,
            reg_count,
            conf[CONF_SKIP_UPDATES],
            conf[CONF_FORCE_NEW_RANGE],
        )
        await cg.register_component(var, conf)
        await sensor.register_sensor(var, conf)

        paren = await cg.get_variable(conf[CONF_MODBUS_CONTROLLER_ID])
        cg.add(paren.add_sensor_item(var))
        await add_modbus_base_properties(var, conf, TDR_TEMP_SENSOR)


