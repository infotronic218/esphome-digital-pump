#include "digital_pump.h"
#include "esphome/core/log.h"
#include<Arduino.h>


namespace esphome {
  namespace digital_pump{
    static const char * TAG ="DOSER";
    DigitalPump::DigitalPump(){
     
    }
    void DigitalPump::dump_config(){
       ESP_LOGCONFIG(TAG, "PUMP Setting ");
       ESP_LOGCONFIG(TAG, "PUMP Connected to GPIO Pin : %d ", this->_pin);
    }
    void DigitalPump::setup(){
        pinMode(this->_pin , OUTPUT);
        digitalWrite(this->_pin, LOW);
        
        
    } 
    static bool started = false ;
    static float onTime = 0 ;
    static unsigned long startedTime ;
    void DigitalPump::loop(){
        //Serial.println("Hello friend");
          float dose =  this->_dose_number->state ;
          float calibration = this->_calibration->state ;
          if(dose!= 0 && calibration!=0){
           float time = dose/calibration ;
             onTime = time ;

            if(!started && this->_doser_on_switch->state){
              started = true ;
              digitalWrite(this->_pin , HIGH);
              startedTime = millis();
              this->_doser_on_switch->publish_state(true);
              ESP_LOGD(TAG, "Turning ON the Pump For : %.2f s", onTime);
              
            }
          }else{
             if(this->_doser_on_switch->state){
              this->_doser_on_switch->publish_state(false);
             }
          }
          float elapsedTime =  (millis()-startedTime )/1000.0;
          if(started && elapsedTime>=onTime ){
            digitalWrite(this->_pin, LOW);
            started =false ;
            this->_doser_on_switch->publish_state(false);
            ESP_LOGD(TAG, "Turning OFF the Pump after : %.2f s ; Elapsed Time : %.2f s", onTime, elapsedTime);
          }
         
          //ESP_LOGD(TAG, "Data %.2f  , %.2f", dose, calibration);
    } 


    void DigitalSwitch::setup(){
       this->pinState = true ;
    }
    void DigitalSwitch::write_state(bool state){
        this->pinState = state ;
        this->publish_state(state);
        
    }
    void DigitalSwitch::dump_config(){
        
    }
  };
};