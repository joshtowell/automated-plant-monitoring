#include <SimpleDHT.h>

#define DHT11_PIN 2
#define MSTSEN_1_PIN A0

//DHT11 Technical Specifications:
// Humidity Range: 20-90% RH
// Humidity Accuracy: ±5%RH
// Temperature Range: 0-50 °C
// Temperature Accuracy: ±2% °C
// Operating Voltage: 3V to 5.5V
SimpleDHT11 dht11(DHT11_PIN);

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
    MoistSensor(byte pin, int dryLimit, int wetLimit) {
      this -> pin = pin;
      this -> dryLimit = dryLimit;
      this -> wetLimit = wetLimit;
      lowCount = 0;
      highCount = 0;
      init();
    }

    void init() {
      pinMode(pin, INPUT);
    }

    void sense() {
      // Read raw analog sensor data
      moistRaw = analogRead(pin);
      // Map value as percentage using known limits
      moistPcnt = map(moistRaw, dryLimit, wetLimit, 0, 100);
//      autoCalibrate();
    }

    void autoCalibrate() {
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

    int getRaw() {
      sense();
      return moistRaw;
    }

    int getPercent() {
      sense();
      return moistPcnt;
    }
};

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
//  senseMoist(wetStored, dryStored);
  Serial.print(MoistSensor1.getPercent());
  Serial.println("%");
  Serial.println(MoistSensor1.getRaw());
  Serial.println();

  // DHT11 sampling rate is 1HZ (1000ms)
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
