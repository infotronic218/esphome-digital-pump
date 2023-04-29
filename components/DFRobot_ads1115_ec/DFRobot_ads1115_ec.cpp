#include "DFRobot_ads1115_ec.h"
#include "esphome/core/log.h"

static const char * TAG ="EC Sensor";


namespace esphome{
 namespace dfrobot_ads1115_ec_{
    static int index = 0 ;
    void DFRobotADS1115EC::dump_config(){
       ESP_LOGCONFIG(TAG, "EC Sensor Setting UP ");
       ESP_LOGCONFIG(TAG, "EC Sensor Connected to Channel : %d ", this->_channel);
       ESP_LOGCONFIG(TAG, "EC Kvalue LOW Calibration is : %.2f ", this->_kvalueLow);
       ESP_LOGCONFIG(TAG, "EC Kvalue HIGH Calibration is : %.2f ", this->_kvalueHigh);
    }

    void DFRobotADS1115EC::setup(){
         this->_kvalue =  this->_kvalueLow;
    }
    
    
    void DFRobotADS1115EC::update(){
        //float  voltage = analogRead(this->_pin) / ESPADC * ESPVOLTAGE; // read the voltage
        //float  voltage = getAnalogRead() / ESPADC * ESPVOLTAGE; // read the voltage
        float  voltage =0, compECsolution;
        if(!_ads1115_adc->is_failed()){
           voltage = _ads1115_adc->analogRead(_channel);
        }else{
            ESP_LOGE(TAG, "The ADS1115 Module is not correctly initialized");
            return ;
        }
        
        voltage = voltage*1000;
        if(this->_calibration_mode_switch->state){
          this->_rawEC = 1000*voltage/RES2/ECREF;
          ESP_LOGD(TAG, "Calibration mode of the EC Sensor ");
          ESP_LOGD(TAG, "Please put the probe into the 1413us/cm or 12.88ms/cm buffer solution");
          ESP_LOGD(TAG, "Copy the following KValue into your configuration");
          if((this->_rawEC>0.9)&&(this->_rawEC<1.9))
          {                         //recognize 1.413us/cm buffer solution
            compECsolution = 1.413*(1.0+0.0185*(this->_temperature-25.0));  //temperature compensation
            ESP_LOGD(TAG, "Recognized 1.413us/cm buffer solution");
          }else if((this->_rawEC>9)&&(this->_rawEC<16.8))
          {                    //recognize 12.88ms/cm buffer solution
            compECsolution = 12.88*(1.0+0.0185*(this->_temperature-25.0));  //temperature compensation
            ESP_LOGD(TAG, "Recognized 12.88ms/cm buffer solution");
          }else 
          {
            ESP_LOGE(TAG, "Calibration Error from Buffer Solution ");
            ESP_LOGE(TAG, "Check the Buffer solution used for the calibration ");
            return ;
          }
          ESP_LOGE(TAG, "Checking the KValue");
          float KValueTemp = RES2*ECREF*compECsolution/1000.0/this->_voltage; 
          if((KValueTemp>0.5) && (KValueTemp<1.5))
          {
            if((this->_rawEC>0.9)&&(this->_rawEC<1.9))
            {
                this->_kvalueLow =  KValueTemp;  
                ESP_LOGD(TAG, "The Calibration KValue LOW is : %.2f", KValueTemp);
            }else if((this->_rawEC>9)&&(this->_rawEC<16.8))
            {
                this->_kvalueHigh =  KValueTemp;
                ESP_LOGD(TAG, "The Calibration KValue HIGH is : %.2f", KValueTemp);
            }
            return ;
          }else{
            ESP_LOGE(TAG, "KValue Error : The KValue is outside the range");
          }
          ESP_LOGD(TAG, "The Calibration KValue is : %.2f", KValueTemp);
          return ;  
        }
        //float  voltage = analogRead(this->_pin) / ESPADC * ESPVOLTAGE; // read the voltage
        float phValue = this->readEC(voltage, this->_temperature);
        this->_ec_sensor->publish_state(phValue) ;
    }

    float DFRobotADS1115EC::readEC(float voltage, float temperature){
        float value = 0,valueTemp = 0;
        this->_rawEC = 1000*voltage/RES2/ECREF;
        valueTemp = this->_rawEC * this->_kvalue;
        //automatic shift process
        //First Range:(0,2); Second Range:(2,20)
        if(valueTemp > 2.5){
            this->_kvalue = this->_kvalueHigh;
        }else if(valueTemp < 2.0){
            this->_kvalue = this->_kvalueLow;
        }

        value = this->_rawEC * this->_kvalue;             //calculate the EC value after automatic shift
        value = value / (1.0+0.0185*(temperature-25.0));  //temperature compensation
        this->_ecvalue = value;                           //store the EC value for Serial CMD calibration
        return value;
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