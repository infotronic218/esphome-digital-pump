#include"tdr_soil_sensor.h"
#include "esphome/core/log.h"
#include <SoftwareSerial.h>
#include <Wire.h>


static const float TEMP_CORRECTION = 0.5;
static const float HUM_CORRECTION = 0.8;
static const float EC_SLOPE = 1.93;
static const float EC_INTERCEPT = -270.8;
static const float EC_TEMP_COEFF = 0.019;
static const char *TAG = "TDR Soil Sensor" ;
static EspSoftwareSerial::UART mod;


namespace esphome {
  namespace  tdr_soil_sensor_{
     void TDR_Soil_Sensor::setup(){
      ESP_LOGI(TAG, "Initialization");
       pinMode(pin_re, OUTPUT);
       pinMode(pin_de, OUTPUT);
       digitalWrite(pin_re, LOW);
       digitalWrite(pin_de, LOW);
       //SoftwareSerial mod(D6, D5);

       mod.begin(38400, SWSERIAL_8N1, pin_rx, pin_tx, false);
       if(!mod){
        ESP_LOGE(TAG, "Error Initializing the Modbus Serial");
        ESP_LOGE(TAG, "Please check  the provided RX, TX pins and try again");
       }else {
        ESP_LOGI(TAG, "Sensor Interface Initialized successfully");
       }

       

     };

     void TDR_Soil_Sensor::dump_config(){
       ESP_LOGCONFIG(TAG, "TDR Soil Sensor Configurations");
       ESP_LOGCONFIG(TAG, "TDR TX Pin : %d ",this->pin_tx);
       ESP_LOGCONFIG(TAG, "TDR RX Pin : %d ",this->pin_rx);
       ESP_LOGCONFIG(TAG, "TDR RE Pin : %d ",this->pin_re);
       ESP_LOGCONFIG(TAG, "TDR DE Pin : %d ",this->pin_de);
     };
  
     void TDR_Soil_Sensor::update(){
         ESP_LOGI(TAG, "Starting measurement");
     };
  };
};