#include <SimpleDHT.h>

const int pinDHT11 = 2;

int wetStored = 262;
int dryStored = 571;
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
  // Intialise serial connection
  Serial.begin(9600);
  // Wait for sensors to intialise
  Serial.println("Initializing sensors...");
  Serial.println();
  delay(3000);
}

void loop() {
  senseTempHumid();
  senseMoist(wetStored, dryStored);
  Serial.println();

  // DHT11 sampling rate is 1HZ
  delay(1000);
}

void senseTempHumid() {
  byte rawTemp = 0;
  byte rawHumid = 0;
  int err = SimpleDHTErrSuccess;
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

void senseMoist(int wetLim, int dryLim) {
  // Read raw analog sensor data
  int rawMoist = analogRead(A0);
  // Map value as percentage using known limits
  int percentMoist = map(rawMoist, wetLim, dryLim, 100, 0);
  Serial.print(percentMoist);
  Serial.println("%");

  // Auto-calibrate 10 continuous low moisture events
  // If below 0% trigger low event
  if (percentMoist < 0) {
    // Store event details
    moistLow = percentMoist;
    moistLowCount++;
    // If 10 events, then set new low limit
    if (moistLowCount > 9 && percentMoist == moistLow) {
      Serial.print("New low moisture detected at ");
      Serial.print(percentMoist);
      Serial.println("%");
      dryStored = rawMoist;
      moistLowCount = 0;
      Serial.println("Low moisture limit successfully re-calibrated");
    }
  }

  // Auto-calibrate 10 continuous high moisture events
  // If above 100% trigger high event
  if (percentMoist > 100) {
    // Store event details
    moistHigh = percentMoist;
    moistHighCount++;
    // If 10 events, then set new low limit
    if (moistHighCount > 9 && percentMoist == moistHigh) {
      Serial.print("New high moisture detected at ");
      Serial.print(percentMoist);
      Serial.println("%");
      wetStored = rawMoist;
      moistHighCount = 0;
      Serial.println("High moisture limit successfully re-calibrated");
    }
  }
}
