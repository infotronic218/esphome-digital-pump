#include "DFRobot_ads1115_ec.h"
#include "esphome/core/log.h"

static const char * TAG ="EC Sensor";
static uint16_t SAMPLES[50];

namespace esphome{
 namespace dfrobot_ads1115_ec_{
    static int index = 0 ;
    void DFRobotADS1115EC::dump_config(){
       ESP_LOGCONFIG(TAG, "PH Sensor Setting UP ");
       ESP_LOGCONFIG(TAG, "PH Sensor Connected to Channel : %d ", this->_channel);
       ESP_LOGCONFIG(TAG, "PH Sensor Acid Voltage Calibration is : %.2f ", this->_acidVoltage);
       ESP_LOGCONFIG(TAG, "PH Sensor Neutral Voltage Calibration is : %.2f ", this->_neutralVoltage);
    }

    void DFRobotADS1115EC::setup(){
        if(!(this->_neutralVoltage> PH_8_VOLTAGE) && (this->_neutralVoltage < PH_6_VOLTAGE)){
            // buffer solution:7.0 is not valid
            ESP_LOGE(TAG, "Neutral Voltage(Solution:7.0) Configuration Error");
            ESP_LOGE(TAG, "Check the Neutral Voltage Calibration");
            ESP_LOGE(TAG, "A valid calibration is range is [%d ,  %d]", PH_8_VOLTAGE, PH_6_VOLTAGE);
        }

        if(!(this->_acidVoltage > PH_5_VOLTAGE) && (this->_acidVoltage < PH_3_VOLTAGE)){
            //buffer solution:4.0
            ESP_LOGE(TAG, "Acid Voltage(Solution:4.0) Configuration Error");
            ESP_LOGE(TAG, "Check the Acid Voltage Calibration");
            ESP_LOGE(TAG, "A valid calibration is range is [%d ,  %d]", PH_5_VOLTAGE, PH_3_VOLTAGE);
            
        }
    }
     float DFRobotADS1115EC::getAnalogRead(){
        float sum = 0 ;
        for(int i=0 ; i<number_of_samples; i++){
            sum += (SAMPLES[i]*1.0)/number_of_samples ;       }
         return sum ;
    }
    void DFRobotADS1115EC::loop1(){
      /* static unsigned long last = millis();
       if(millis()-last > 200){
           // SAMPLES[index]= analogRead(this->_pin);
            index ++ ;
            if(index >=number_of_samples){
                index = 0 ;
            }
            last = millis();
        }*/
    }
    void DFRobotADS1115EC::update(){
        //float  voltage = analogRead(this->_pin) / ESPADC * ESPVOLTAGE; // read the voltage
        //float  voltage = getAnalogRead() / ESPADC * ESPVOLTAGE; // read the voltage
        float  voltage =0;
        if(!_ads1115_adc->is_failed()){
           voltage = _ads1115_adc->analogRead(_channel);
        }else{
            ESP_LOGE(TAG, "The ADS1115 Module is not correctly initialized");
            return ;
        }
        
        voltage = voltage*1000;
        if(this->_calibration_mode_switch->state){
          ESP_LOGD(TAG, "Calibration mode of the PH Sensor ");
          ESP_LOGD(TAG, "Use and Acid or Neutral solution for calibration");
          ESP_LOGD(TAG, "Copy the following Voltage corresponding to your Acid or Neutral Voltage Calibration");
          ESP_LOGD(TAG, "The Calibration voltage is : %.2f", voltage);
          return ;  
        }
        //float  voltage = analogRead(this->_pin) / ESPADC * ESPVOLTAGE; // read the voltage
        float phValue = this->readPH(voltage, this->_temperature);
        this->_ph_sensor->publish_state(phValue) ;
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