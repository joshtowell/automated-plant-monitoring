#include <SimpleDHT.h>

const int pinDHT11 = 2;
const int wetVal = 262;
const int dryVal = 571;

//DHT11 Technical Specifications:
// Humidity Range: 20-90% RH
// Humidity Accuracy: ±5%RH
// Temperature Range: 0-50 °C
// Temperature Accuracy: ±2% °C
// Operating Voltage: 3V to 5.5V
SimpleDHT11 dht11(pinDHT11);

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing sensors...");
  Serial.println();
  delay(3000);
}

void loop() {
  senseTempHumid();
  senseMoist();
  Serial.println();

  // DHT11 sampling rate is 1HZ
  delay(1000);
}

void senseTempHumid() {
  byte rawTemp = 0;
  byte rawHumid = 0;
  int err = SimpleDHTErrSuccess;
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

void senseMoist() {
  int rawMoist = analogRead(A0);
  int percentMoist = map(rawMoist, wetVal, dryVal, 100, 0);
  Serial.print(percentMoist);
  Serial.println("%");
}
