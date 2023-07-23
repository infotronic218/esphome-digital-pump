#ifndef __TDR_SOIL_SENSOR__
#define __TDR_SOIL_SENSOR__

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include<Arduino.h>

namespace esphome {
  namespace  tdr_soil_sensor_{
     class TDR_Soil_Sensor : public PollingComponent{
       private:
       sensor::Sensor temperature_sensor_ ;
       sensor::Sensor humidity_sensor_ ;
       sensor::Sensor ec_sensor_ ;
       uint8_t pin_tx;
       uint8_t pin_rx ;
       uint8_t pin_de ;
       uint8_t pin_re ;
       public:
       void setup() override;
       void update() override ;
       void dump_config() override;
       float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }
       void set_temperature_sensor(sensor::Sensor s){this->temperature_sensor_ = s ;}
       void set_humidity_sensor(sensor::Sensor s ){this->humidity_sensor_ = s ;}
       void set_ec_sensor(sensor::Sensor s) {this->ec_sensor_ = s ;}

       void set_pin_tx(uint8_t pin ){this->pin_tx = pin ;}
       void set_pin_rx(uint8_t pin ){this->pin_rx = pin ;}
       void set_pin_de(uint8_t pin ){this->pin_de = pin ;}
       void set_pin_re(uint8_t pin ){this->pin_re = pin ;}
    

     };
  };
};

#endif