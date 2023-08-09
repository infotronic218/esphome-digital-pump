#include "tdr_soil_sensor.h"
#include "esphome/core/log.h"
static const char *TAG = "TDR Soil Sensor" ;
static const char* TAG_TEMP = "tdr_temperature_sensor ";
static const char* TAG_EC = "tdr_ec_sensor ";
static const char* TAG_WC = "tdr_wc_sensor ";
namespace esphome {
    namespace tdr_soil_sensor__{

            void TDR_Soil_Sensor::setup(){
                ESP_LOGI(TAG, "Initialization");
            }
            void TDR_Soil_Sensor::update() {

            }
            void TDR_Soil_Sensor::dump_config(){
                
            };
          
          void TDR_Temp_Sensor::dump_config(){LOG_SENSOR(TAG_TEMP, "Modbus Controller Sensor", this);};

          void TDR_Temp_Sensor::parse_and_publish(const std::vector<uint8_t> &data) {
              
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

            result = result/10.0 ;
            ESP_LOGD(TAG_TEMP, "Sensor new state: %.02f", result);
            // this->sensor_->raw_state = result;
            this->publish_state(result);


        }



        void TDR_EC_Sensor::parse_and_publish(const std::vector<uint8_t> &data) {
              
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

            result = result/100.0 ;
            ESP_LOGD(TAG_EC, "Sensor new state: %.02f", result);
            // this->sensor_->raw_state = result;
            this->publish_state(result);


        }
        void TDR_EC_Sensor::dump_config() { LOG_SENSOR(TAG_EC, "Modbus Controller Sensor", this); }


        void TDR_WC_Sensor::parse_and_publish(const std::vector<uint8_t> &data){
              
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
            result = result/10.0 ;
            ESP_LOGD(TAG_WC, "Sensor new state: %.02f", result);
            // this->sensor_->raw_state = result;
            this->publish_state(result);


        }

        void TDR_WC_Sensor::dump_config() { LOG_SENSOR(TAG_WC, "Modbus Controller Sensor", this); }

    };

};
