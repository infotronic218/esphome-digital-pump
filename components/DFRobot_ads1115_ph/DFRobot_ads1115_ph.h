#ifndef __DFRobot_ESP_PH__
#define __DFRobot_ESP_PH__


#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/ads1115_adc/ads1115_adc.h"
#include<Arduino.h>

#define PH_8_VOLTAGE 1122
#define PH_6_VOLTAGE 1478
#define PH_5_VOLTAGE 1654
#define PH_3_VOLTAGE 2010

#define ESPADC 4096.0   //the esp Analog Digital Convertion value
#define ESPVOLTAGE 3300 //the esp voltage supply value

namespace esphome{
    namespace dfrobot_ads1115_ph_ {
         class DigitalSwitch;
         class DFRobotADS1115PH : public PollingComponent{
            protected:
            uint8_t _channel ;
            float _phValue;
            float _acidVoltage;
            float _neutralVoltage;
            float _voltage;
            float _temperature;
         
            sensor::Sensor *_ph_sensor{nullptr} ;
            dfrobot_ads1115_ph_::DigitalSwitch *_calibration_mode_switch{nullptr} ;
            ads1115_adc_::ADS1115_ADC * _ads1115_adc;
            float getAnalogRead();
            public: 
            void setup()override ;
            void update() override ;
            void dump_config() override;
            float get_setup_priority() const override { return esphome::setup_priority::LATE-100; }
            float readPH(float voltage, float temperature); // voltage to pH value, with temperature compensation
            void set_channel (uint8_t ch){this->_channel = ch ;}
            void set_temperature(float t){this->_temperature = t ;}
            void set_cal_acidVoltage(float v){this->_acidVoltage = v ;}
            void set_cal_neutralVoltage(float v){this->_neutralVoltage = v ;}
            void set_calibration_mode_switch(dfrobot_ads1115_ph_::DigitalSwitch  *sw){this->_calibration_mode_switch = sw ;}
            void set_ph_sensor(sensor::Sensor *s){this->_ph_sensor = s ;}
            void set_ads1115(ads1115_adc_::ADS1115_ADC *ads){this->_ads1115_adc = ads ;}
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