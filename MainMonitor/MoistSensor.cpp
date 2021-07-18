#include "MoistSensor.h"

MoistSensor::MoistSensor(byte pin, int dryLimit, int wetLimit) {
  this -> pin = pin;
  this -> dryLimit = dryLimit;
  this -> wetLimit = wetLimit;
  lowCount = 0;
  highCount = 0;
  init();
}

void MoistSensor::init() {
  pinMode(pin, INPUT);
  sense();
}

void MoistSensor::sense() {
  // Read raw analog sensor data
  moistRaw = analogRead(pin);
  // Map value as percentage using known limits
  moistPcnt = map(moistRaw, dryLimit, wetLimit, 0, 100);
//      autoCalibrate();
}

void MoistSensor::autoCalibrate() {
  // Auto-calibrate 10 continuous low moisture events
  // If below 0% trigger low event
  if (moistPcnt < 0) {
    // Store event details
    lowLastPcnt = moistPcnt;
    lowCount++;
    if (lowCount == 1) lowTime = millis();
    // If 10 events in 20s, then set new low limit
    if (lowCount > 9 && (millis() - lowTime) <= 20000 && moistPcnt == lowLastPcnt) {
      Serial.print("New low moisture detected at ");
      Serial.print(moistPcnt);
      Serial.println("%");
      dryLimit = moistRaw;
      lowCount = 0;
      Serial.println("Low moisture limit successfully re-calibrated");
    } else {
      lowTime = 0;
    }
  }

  // Auto-calibrate 10 continuous high moisture events
  // If above 100% trigger high event
  if (moistPcnt > 100) {
    // Store event details
    highLastPcnt = moistPcnt;
    highCount++;
    if (highCount == 1) highTime = millis();
    // If 10 events in 20s, then set new low limit
    if (highCount > 9 && (millis() - highTime) <= 20000 && moistPcnt == highLastPcnt) {
      Serial.print("New high moisture detected at ");
      Serial.print(moistPcnt);
      Serial.println("%");
      wetLimit = moistRaw;
      highCount = 0;
      Serial.println("High moisture limit successfully re-calibrated");
    } else {
      highTime = 0;
    }
  }
}

int MoistSensor::getRaw() {
  sense();
  return moistRaw;
}

int MoistSensor::getPercent() {
  sense();
  return moistPcnt;
}
