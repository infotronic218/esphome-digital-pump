#include "digital_pump.h"
#include "esphome/core/log.h"
#include<Arduino.h>


namespace esphome {
  namespace digital_pump{
    static const char * TAG ="DOSER";
    static Servo servo ;
    DigitalPump::DigitalPump(){
     
    }
    void DigitalPump::dump_config(){
       ESP_LOGCONFIG(TAG, "PUMP Setting ");
       ESP_LOGCONFIG(TAG, "PUMP Connected to GPIO Pin : %d ", this->_pin);
       ESP_LOGCONFIG(TAG, "PUMP Speed Set to : %.2f ", this->_speed);
       const char *rot = this->_clockwise_rotation? "CLOCKWISE" : "COUNTER CLOCKWISE";
       ESP_LOGCONFIG(TAG, "PUMP Rotation Set to : %s ", rot);
    }
    void DigitalPump::setup(){
      servo.attach(this->_pin);
        
    } 
    static bool started = false ;
    static float onTime = 0 ;
    static unsigned long startedTime ;
    static bool lastMode = false ;
    void DigitalPump::loop(){
        float realSpeed = 0 ;
        if(this->_clockwise_rotation){
           realSpeed = 90 - this->_speed ;
        }else {
          realSpeed = 90 + this->_speed ;
        }
       
          if(lastMode != this->_auto_mode_switch->state){
                 const char * mode = this->_auto_mode_switch->state? "AUTO":"MANUAL";
                  servo.write(90);
                  this->_doser_on_switch->publish_state(false);
                  ESP_LOGD(TAG, "Mode changed to : %s", mode );
                  started= false ;
          }
          if(this->_auto_mode_switch->state) {
              float dose =  this->_dose_number->state ;
              float calibration = this->_calibration->state ;
              if(dose!= 0 && calibration!=0){
              float time = dose/calibration ;
                onTime = time ;

                if(!started && this->_doser_on_switch->state){
                  started = true ;
                  servo.write(realSpeed);
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
                servo.write(90);
                started =false ;
                this->_doser_on_switch->publish_state(false);
                ESP_LOGD(TAG, "Turning OFF the Pump after : %.2f s ; Elapsed Time : %.2f s", onTime, elapsedTime);
              }
          } else {
            if(this->_doser_on_switch->state && !started){
                  servo.write(realSpeed);
                  this->_doser_on_switch->publish_state(true);
                  ESP_LOGD(TAG, "Turning ON the Pump Manual Mode");
                  started = true ;
            }else if(!this->_doser_on_switch->state && started){
                  servo.write(90);
                  this->_doser_on_switch->publish_state(false);
                  ESP_LOGD(TAG, "Turning OFF the Pump Manual Mode");
                  started= false ;
            }
          }

          lastMode =  this->_auto_mode_switch->state;  
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