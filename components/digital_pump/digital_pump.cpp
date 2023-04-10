#include "digital_pump.h"
#include<Arduino.h>


namespace esphome {
  namespace digital_pump{
    DigitalPump::DigitalPump(){

    }
    void DigitalPump::setup(){
        pinMode(this->_pin , OUTPUT);
        digitalWrite(this->_pin, LOW);
    } 
    void DigitalPump::loop(){

    } 
  };
};