#include <SimpleDHT.h>
#include "MoistSensor.h"
//#include "TempHumidSensor.h"

#define DHT11_PIN 2
#define MSTSEN_1_PIN A0

//DHT11 Technical Specifications:
// Humidity Range: 20-90% RH
// Humidity Accuracy: ±5%RH
// Temperature Range: 0-50 °C
// Temperature Accuracy: ±2% °C
// Operating Voltage: 3V to 5.5V
SimpleDHT11 dht11(DHT11_PIN);

//TempHumidSensor(DHT11_PIN);
MoistSensor MoistSensor1(MSTSEN_1_PIN, 655, 294);

void setup() {
  // Intialise serial connection
  Serial.begin(9600);
  // Wait for sensors to intialise
  Serial.println("Initializing sensors...");
  Serial.println();
  delay(3000);
}

void loop() {
  // Get and print all sensor data
  senseTempHumid();
  Serial.print(MoistSensor1.getPercent());
  Serial.println("%");
//  Serial.println(MoistSensor1.getRaw());
  Serial.println();

  // DHT11 sampling rate is 1HZ (1000ms)
  delay(1000);
}

void senseTempHumid() {
  byte rawTemp = 0;
  byte rawHumid = 0;
  // Read raw digital sensor data
  // Catch sensor exceptions and display errors
  if ((err = dht11.read(&rawTemp, &rawHumid, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.print(SimpleDHTErrCode(err));
    Serial.print(",");
    Serial.println(SimpleDHTErrDuration(err));
    delay(1000);
    return;
  } else {
    Serial.print((int)rawTemp);
    Serial.println(" C");
    Serial.print((int)rawHumid);
    Serial.println(" H");
  }
}
