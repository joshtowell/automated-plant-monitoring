#ifndef TEMP_HUMID_SENSOR_H
#define TEMP_HUMID_SENSOR_H

#include <Arduino.h>

class TempHumidSensor {
  
  private:
    byte id;
    byte pin;
    int error;
    int tempCelsius;
    int humidRelative;
    
  public:
    TempHumidSensor(byte id, byte pin);
    void init();
    void sense();
    int getTemp(char unit);
    int getHumid();
    
};

#endif
