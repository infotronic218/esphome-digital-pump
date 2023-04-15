# ESPHOME Digital pump


* Adding the PUMP to the configuration 
``` yaml

digital_pump:
  - id : Pump1
    pin: 2
    speed:  90 # [0.0 - 90.0]  
    clockwise_rotation: true # [true for clockwise rotation or false  for counter clockwise rotation]
    auto_mode: # switch the operation mode
      name: "Doser Auto Mode"
    doser_on: # switch for the doser
      name: "Doser ON"
    dose: # Number for the dose in (L)
      name: "Pump 1 dose (L)"
      mode: box
      min_value: 0
      max_value: 1000
    calibration: # Number for the dose calibration (L/s)
      name: "Pump 1 Calibration (L/s)"
      mode: box
      min_value: 0
      max_value: 100000
```

* Adding the PH sensor to the configuration 

``` yaml
DFRobot_ESP_PH:
  - id: PH1
    pin: 35  # ADC pin Connected to the PH Sensor
    acid_voltage: 1000      # Calibration for Acid solution (4.0)
    neutral_voltage: 15255  # Calibration for Neutral solution (7.0)
    temperature: 25 
    update_interval: 10s
    calibration_mode: # Switch to select calibration mode 
      name: "Calibration mode"
    ph_sensor: # The PH sensor holding the read value
      name: "PH sensor 1"
```