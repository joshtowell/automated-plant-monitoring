#include "MoistSensor.h"

MoistSensor::MoistSensor(byte id, byte pin, int dryLimit, int wetLimit) {
  this -> id = id;
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
//  autoCalibrate();
}


void MoistSensor::calibrate() {
  const int calibLen = 20;
  int tmpSum = 0;
  Serial.print("[*] Beginning calibration for moisture sensor ");
  Serial.print(id);
  Serial.println(" ...");
  Serial.print("[!] Starting ");
  Serial.print(calibLen);
  Serial.println(" sec dry calibration in...");
  for (byte i = 8; i > 0; i--) {
    Serial.println(i);
    delay(1000);
  }
  Serial.print("[");
  tmpSum = 0;
  for (byte i = 0; i < calibLen; i++) {
    sense();
    tmpSum += moistRaw;
    delay(1000);
    if ((i + 1) % 10 == 0) Serial.print(i + 1);
    else Serial.print(".");
  }
  dryLimit = tmpSum / calibLen;
  Serial.println("]");
  Serial.print("[!] Starting ");
  Serial.print(calibLen);
  Serial.println(" sec wet calibration in...");
  for (byte i = 8; i > 0; i--) {
    Serial.println(i);
    delay(1000);
  }
  Serial.print("[");
  tmpSum = 0;
  for (byte i = 0; i < calibLen; i++) {
    sense();
    tmpSum += moistRaw;
    delay(1000);
    if ((i + 1) % 10 == 0) Serial.print(i + 1);
    else Serial.print(".");
  }
  wetLimit = tmpSum / calibLen;
  Serial.println("]");
  Serial.println();
  Serial.println("[✓] All calibration complete");
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
    if (lowCount > 5 && (millis() - lowTime) <= 20000 && moistPcnt == lowLastPcnt) {
      Serial.print("[!] New low moisture detected at ");
      Serial.print(moistPcnt);
      Serial.print("%");
      Serial.print(" (");
      Serial.print(moistRaw);
      Serial.println(")");
      dryLimit = moistRaw;
      lowCount = 0;
      Serial.println("[✓] Low moisture limit successfully re-calibrated");
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
    if (highCount > 5 && (millis() - highTime) <= 20000 && moistPcnt == highLastPcnt) {
      Serial.print("[!] New high moisture detected at ");
      Serial.print(moistPcnt);
      Serial.print("%");
      Serial.print(" (");
      Serial.print(moistRaw);
      Serial.println(")");
      wetLimit = moistRaw;
      highCount = 0;
      Serial.println("[✓] High moisture limit successfully re-calibrated");
    } else {
      highTime = 0;
    }
  }
}

int MoistSensor::getId() {
  return id;
}

int MoistSensor::getRaw() {
  sense();
  return moistRaw;
}

int MoistSensor::getPercent() {
  sense();
  return moistPcnt;
}
