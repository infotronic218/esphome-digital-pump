#ifndef __SOIL_SENSOR__
#define __SOIL_SENSOR__
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "modbus_sensor.h"
#include "esphome/core/log.h"
//#include "esphome/components/switch/switch.h"
static const char* TAG_TEMP = "TEMP Sensor";
static const char* TAG_EC = "EC Sensor";
static const char* TAG_WC = "WC Sensor";
namespace esphome{
namespace tdr_soil_sensor_{
    class TDR_Temp_Sensor;
    class TDR_Soil_Sensor :public PollingComponent {
         private:
            tdr_soil_sensor_::TDR_Temp_Sensor *temperature_sensor_ ;
            tdr_soil_sensor_::TDR_Temp_Sensor *humidity_sensor_ ;
            tdr_soil_sensor_::TDR_Temp_Sensor *ec_sensor_ ;
            tdr_soil_sensor_::TDR_Temp_Sensor *pore_water_sensor_ ;
            tdr_soil_sensor_::TDR_Temp_Sensor *bulk_permittivity_sensor_ ; 
            public:
            void setup() override;
            void update() override ;
            void dump_config() override;
            float get_setup_priority() const override { return  -200; }
            void set_temperature_sensor(tdr_soil_sensor_::TDR_Temp_Sensor *s){this->temperature_sensor_ = s ;}
            void set_humidity_sensor(tdr_soil_sensor_::TDR_Temp_Sensor *s ){this->humidity_sensor_ = s ;}
            void set_ec_sensor(tdr_soil_sensor_::TDR_Temp_Sensor *s) {this->ec_sensor_ = s ;}
            void set_pore_water_sensor(tdr_soil_sensor_::TDR_Temp_Sensor *s) {this->pore_water_sensor_ = s ;}
            void set_bulk_permittivity_sensor(tdr_soil_sensor_::TDR_Temp_Sensor *s) {this->bulk_permittivity_sensor_ = s ;}

    };

    class TDR_Temp_Sensor: public modbus_controller::ModbusSensor{
      
        void parse_and_publish(const std::vector<uint8_t> &data) override{
              
            float result = payload_to_float(data, *this);
            // Is there a lambda registered
            // call it with the pre converted value and the raw data array
            if (this->transform_func_.has_value()) {
                // the lambda can parse the response itself
                auto val = (*this->transform_func_)(this, result, data);
                if (val.has_value()) {
                ESP_LOGV(TAG_TEMP, "Value overwritten by lambda");
                result = val.value();
                }
            }

            result = result/100.0 ;
            ESP_LOGD(TAG_TEMP, "Sensor new state: %.02f", result);
            // this->sensor_->raw_state = result;
            this->publish_state(result);


        }

    };



    class TDR_EC_Sensor: public modbus_controller::ModbusSensor{
      
        void parse_and_publish(const std::vector<uint8_t> &data) override{
              
            float result = payload_to_float(data, *this);
            // Is there a lambda registered
            // call it with the pre converted value and the raw data array
            if (this->transform_func_.has_value()) {
                // the lambda can parse the response itself
                auto val = (*this->transform_func_)(this, result, data);
                if (val.has_value()) {
                ESP_LOGV(TAG_EC, "Value overwritten by lambda");
                result = val.value();
                }
            }

            result = result/100 ;
            ESP_LOGD(TAG_EC, "Sensor new state: %.02f", result);
            // this->sensor_->raw_state = result;
            this->publish_state(result);


        }

    };




    class TDR_WC_Sensor: public modbus_controller::ModbusSensor{
      
        void parse_and_publish(const std::vector<uint8_t> &data) override{
              
            float result = payload_to_float(data, *this);
            // Is there a lambda registered
            // call it with the pre converted value and the raw data array
            if (this->transform_func_.has_value()) {
                // the lambda can parse the response itself
                auto val = (*this->transform_func_)(this, result, data);
                if (val.has_value()) {
                ESP_LOGV(TAG_WC, "Value overwritten by lambda");
                result = val.value();
                }
            }
            result = result/10 ;
            ESP_LOGD(TAG_WC, "Sensor new state: %.02f", result);
            // this->sensor_->raw_state = result;
            this->publish_state(result);


        }

    };



};

};

#endif