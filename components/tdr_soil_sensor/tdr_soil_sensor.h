#ifndef __TDR_SOIL_SENSOR__
#define __TDR_SOIL_SENSOR__

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/ads1115_adc/ads1115_adc.h"
#include<Arduino.h>

namespace esphome {
  namespace  tdr_soil_sensor_{
     class TDR_Soil_Sensor : public PollingComponent{
       private:
       sensor::Sensor temperature_sensor_ ;
       sensor::Sensor humidity_sensor_ ;
       sensor::Sensor ec_sensor_ ;

       public:
       void setup(void) override;
       void dumps_config(void) override;
       void update_interval() override ;
       void set_temperature_sensor(sensor::Sensor s){this->temperature_sensor_ = s ;}
       void set_humidity_sensor(sensor::Sensor s ){this->humidity_sensor_ = s ;}
       void set_ec_sensor(sensor::Sensor s) {this->ec_sensor_ = s ;}
    

     }
  };
};

#endif