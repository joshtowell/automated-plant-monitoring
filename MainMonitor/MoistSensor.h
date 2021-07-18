#ifndef MOIST_SENSOR_H
#define MOIST_SENSOR_H

#include <Arduino.h>

class MoistSensor {
  
  private:
    byte pin;
    
    byte lowCount;
    int lowLastPcnt;
    int lowTime;
    
    byte highCount;
    int highLastPcnt;
    int highTime;
    
    int dryLimit;
    int wetLimit;
    int moistRaw;
    int moistPcnt;
    
  public:
    MoistSensor(byte pin, int dryLimit, int wetLimit);
    void init();
    
    void sense();
    void autoCalibrate();
    
    int getRaw();
    int getPercent();
    
};

#endif
