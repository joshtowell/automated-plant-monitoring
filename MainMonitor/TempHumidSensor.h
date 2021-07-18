#ifndef TEMP_HUMID_SENSOR_H
#define TEMP_HUMID_SENSOR_H

#include <Arduino.h>

class TempHumidSensor {
  
  private:
    byte pin;
    int error;
    int tempCelsius;
    int humidRelative;
    
  public:
    TempHumidSensor(byte pin);
    void init();
    void sense();
    int getTemp(char unit);
    int getHumid();
    
};

#endif
