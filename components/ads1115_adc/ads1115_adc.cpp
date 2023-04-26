#include "ads1115_adc.h"
#include "esphome/core/log.h"

#include <Wire.h>

static const char * TAG ="ADS1115";

namespace esphome{
    namespace ads1115_adc_ {
             void ADS1115_ADC::setup(){
                Wire.begin((int)this->sda_, (int)this->scl_);
                bool ret =  adafruit_ADS1X15.begin(this->addr_, &Wire);
                if(!ret){
                    ESP_LOGE(TAG, "Failled to initialize the ADS1115 Module");
                    this->mark_failed();
                }
             }
             void ADS1115_ADC::dump_config(){
                 ESP_LOGCONFIG(TAG, "ADS1115 ADC Setting UP ");
                 ESP_LOGCONFIG(TAG, "SDA PIN : %d ", this->sda_);
                 ESP_LOGCONFIG(TAG, "SCL PIN : %d ", this->scl_);
                 ESP_LOGCONFIG(TAG, "I2C Address : %x ", this->addr_);
                 
             }

             float ADS1115_ADC::analogRead(uint8_t ch){
                 uint16_t adc = adafruit_ADS1X15.readADC_SingleEnded(ch);
                 float volts = adafruit_ADS1X15.computeVolts(adc);
                 return volts ;
             }
    };
    
};