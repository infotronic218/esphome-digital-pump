#ifndef __digital_pump__
#define __digital_pump__
#include "esphome/core/component.h"

namespace esphome {
  namespace digital_pump{
     class DigitalPump: public Component {
        private:
         uint8_t _pin ;
         float _dose ;
         float _calibration ;
         public:
            DigitalPump();
            void setup() override ;
            void loop() override ;
            float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }
            void set_pin(uint8_t pin){this->_pin = pin;}
            void set_dose(float dose){this->_dose = dose ;}
            void set_calibration(float calibration){this->_calibration = calibration;}
     };
  };
};

#endif 