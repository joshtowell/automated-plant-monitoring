#include <SimpleDHT.h>

const int pinDHT11 = 2;

int wetVal = 262;
int dryVal = 571;
int moistLow = 0;
int moistLowCount = 0;
int moistHigh = 0;
int moistHighCount = 0;

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

  // Auto-calibrate 10 continuous low moisture events
  if (percentMoist < 0) {
    moistLow = percentMoist;
    moistLowCount++;
    if (moistLowCount > 9 && percentMoist == moistLow) {
      Serial.print("New low moisture detected at ");
      Serial.print(percentMoist);
      Serial.println("%");
      dryVal = rawMoist;
      moistLowCount = 0;
      Serial.println("Low moisture limit successfully re-calibrated");
    }
  }

  // Auto-calibrate 10 continuous high moisture events
  if (percentMoist > 100) {
    moistHigh = percentMoist;
    moistHighCount++;
    if (moistHighCount > 9 && percentMoist == moistHigh) {
      Serial.print("New high moisture detected at ");
      Serial.print(percentMoist);
      Serial.println("%");
      wetVal = rawMoist;
      moistHighCount = 0;
      Serial.println("High moisture limit successfully re-calibrated");
    }
  }
}
