#include <SimpleDHT.h>
#include "TempHumidSensor.h"

TempHumidSensor::TempHumidSensor(byte id, byte pin) {
  this -> id = id;
  this -> pin = pin;
  error = SimpleDHTErrSuccess;
  init();
}

void TempHumidSensor::init() {
  sense();
}

void TempHumidSensor::sense() {
  // Read raw digital sensor data
  // Catch sensor exceptions and display errors
//  if ((error = dht11.read(&tempCelsius, &humidRelative, NULL)) != SimpleDHTErrSuccess) {
//    Serial.print("[!] Read DHT11 failed, err=");
//    Serial.print(SimpleDHTErrCode(error));
//    Serial.print(",");
//    Serial.println(SimpleDHTErrDuration(error));
//    delay(1000);
//    return;
//  }
}

int TempHumidSensor::getTemp(char unit = 'c') {
  sense();
  if (unit == 'c') return tempCelsius;
  else if (unit == 'f') return ((tempCelsius * 1.8) + 32);
  else return NULL;
}

int TempHumidSensor::getHumid() {
  sense();
  return humidRelative;
}
