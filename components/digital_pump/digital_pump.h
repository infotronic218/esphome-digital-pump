#ifndef __digital_pump__
#define __digital_pump__
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "template_number.h"
#include "esphome/components/switch/switch.h"
#include "ESP32Servo.h"
namespace esphome {
  namespace digital_pump{
     class DigitalSwitch;
     class DigitalPump: public Component {
        private:
         uint8_t _pin ;
         float _speed ;
         bool _clockwise_rotation ;
         digital_pump::TemplateNumber  * _calibration{nullptr} ;
         digital_pump::TemplateNumber  *_dose_number{nullptr} ; 
         digital_pump::DigitalSwitch * _doser_on_switch {nullptr};
         digital_pump::DigitalSwitch * _auto_mode_switch {nullptr};
         public:
            DigitalPump();
            void setup() override ;
            void loop() override ;
            void dump_config() override;
            float get_setup_priority() const override { return esphome::setup_priority::LATE; }
            void set_pin(uint8_t pin){this->_pin = pin;}
            void set_speed(float sp){this->_speed = sp;}
            void set_clockwise_rotation(bool rot){this->_clockwise_rotation = rot;}
            void set_calibration_number(digital_pump::TemplateNumber *num){this->_calibration = num;}
            void set_dose_number(digital_pump::TemplateNumber *num){this->_dose_number = num ; }
            void set_dose_on_switch( digital_pump::DigitalSwitch *sw){this->_doser_on_switch = sw ; }
            void set_auto_mode_switch( digital_pump::DigitalSwitch *sw){this->_auto_mode_switch = sw ; }
     };


     class DigitalSwitch: public switch_::Switch, public Component{
      public:
      DigitalSwitch(){

      }


      void setup() override;
      void write_state(bool state) override;
      void dump_config() override;

      bool pinState ;
     };


     
  };

  
};

#endif 