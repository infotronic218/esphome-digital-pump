# ESPHOME Digital pump


* Adding the PUMP to the configuration 
``` yaml
digital_pump:
  - id : Pump1
    pin: 2
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
