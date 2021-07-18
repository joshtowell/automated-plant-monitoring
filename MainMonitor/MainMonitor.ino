#include <SimpleDHT.h>
#include "MoistSensor.h"
//#include "TempHumidSensor.h"

#define DHT11_PIN 2
#define MSTSEN_1_PIN A0
#define MSTSEN_2_PIN A1
#define MSTSEN_3_PIN A2

int err = SimpleDHTErrSuccess;

//DHT11 Technical Specifications:
// Humidity Range: 20-90% RH
// Humidity Accuracy: ±5%RH
// Temperature Range: 0-50 °C
// Temperature Accuracy: ±2% °C
// Operating Voltage: 3V to 5.5V
SimpleDHT11 dht11(DHT11_PIN);

//TempHumidSensor(1, DHT11_PIN);
MoistSensor MoistSensor1(1, MSTSEN_1_PIN, 612, 365); // Last calibrated 21-07-18 19:20
MoistSensor MoistSensor2(2, MSTSEN_2_PIN, 625, 370); // Last calibrated 21-07-18 19:20
MoistSensor MoistSensor3(3, MSTSEN_3_PIN, 615, 370); // Last calibrated 21-07-18 19:20

void setup() {
  // Intialise serial connection
  Serial.begin(9600);
  // Wait for sensors to intialise
  Serial.println("[*] Initializing sensors...");
  Serial.println();
  delay(3000);
}

void loop() {
  // Get and print all sensor data
  senseTempHumid();
  senseMoisture();
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
    Serial.print("[!] Read DHT11 failed, err=");
    Serial.print(SimpleDHTErrCode(err));
    Serial.print(",");
    Serial.println(SimpleDHTErrDuration(err));
    delay(1000);
    return;
  } else {
    Serial.print("Temp: ");
    Serial.print((int)rawTemp);
    Serial.println(" C");
    Serial.print("Humid: ");
    Serial.print((int)rawHumid);
    Serial.println(" %");
  }
}

void senseMoisture() {
  Serial.print("MS-");
  Serial.print(MoistSensor1.getId());
  Serial.print(": ");
  Serial.print(MoistSensor1.getPercent());
  Serial.println("%");
  
  Serial.print("MS-");
  Serial.print(MoistSensor2.getId());
  Serial.print(": ");
  Serial.print(MoistSensor2.getPercent());
  Serial.println("%");
  
  Serial.print("MS-");
  Serial.print(MoistSensor3.getId());
  Serial.print(": ");
  Serial.print(MoistSensor3.getPercent());
  Serial.println("%");
}
