#include "esphome/core/component.h"
#include<Arduino.h>
#include<Wire.h>
#include <Adafruit_ADS1X15.h>


namespace esphome{
    namespace ads1115_adc_ {

        class ADS1115_ADC : public Component{
            private:
             Adafruit_ADS1X15 adafruit_ADS1X15 ;
             uint8_t sda_ ;
             uint8_t scl_ ;
             uint8_t addr_ ;

            public:
             void setup() override ;
             void dump_config() override;
             float get_setup_priority() const override { return esphome::setup_priority::LATE; }
             void set_sda_pin(uint8_t pin) {this->sda_ = pin ;}
             void set_scl_pin(uint8_t pin) {this->scl_ = pin ;}
             void set_address(uint8_t addr) {this->addr_ = addr ;}
             float analogRead(uint8_t ch);
        };

    };

};