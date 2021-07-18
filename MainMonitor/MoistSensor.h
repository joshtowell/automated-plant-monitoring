#ifndef MOIST_SENSOR_H
#define MOIST_SENSOR_H

#include <Arduino.h>

class MoistSensor {
  
  private:
    byte id;
    byte pin;
    
    byte lowCount;
    int lowLastPcnt;
    unsigned long lowTime;
    
    byte highCount;
    int highLastPcnt;
    unsigned long highTime;
    
    int dryLimit;
    int wetLimit;
    int moistRaw;
    int moistPcnt;
    
  public:
    MoistSensor(byte id, byte pin, int dryLimit, int wetLimit);
    void init();
    
    void sense();
    void calibrate();
    void autoCalibrate();

    int getId();
    int getRaw();
    int getPercent();
    
};

#endif
