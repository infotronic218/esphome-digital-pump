#include "DFRobot_ESP_PH.h"
#include "esphome/core/log.h"

static const char * TAG ="PH Sensor";
namespace esphome{
 namespace dfrobot_esp_ph_{
    void DFRobotPH::dump_config(){
       ESP_LOGCONFIG(TAG, "PH Sensor Setting UP ");
       ESP_LOGCONFIG(TAG, "PH Sensor Connected to Pin : %d ", this->_pin);
       ESP_LOGCONFIG(TAG, "PH Sensor Acid Voltage Calibration is : %.2f ", this->_acidVoltage);
       ESP_LOGCONFIG(TAG, "PH Sensor Neutral Voltage Calibration is : %.2f ", this->_neutralVoltage);
    }

    void DFRobotPH::setup(){
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
    void DFRobotPH::update(){
        float  voltage = analogRead(this->_pin) / ESPADC * ESPVOLTAGE; // read the voltage
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

    float DFRobotPH::readPH(float voltage, float temperature){
        float slope = (7.0 - 4.0) / ((this->_neutralVoltage - 1500.0) / 3.0 - (this->_acidVoltage - 1500.0) / 3.0); // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
        float intercept = 7.0 - slope * (this->_neutralVoltage - 1500.0) / 3.0;
        this->_phValue = slope * (voltage - 1500.0) / 3.0 + intercept; //y = k*x + b
        return this->_phValue;
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