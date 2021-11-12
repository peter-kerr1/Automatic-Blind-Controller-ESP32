#define BLIND_NAME "blind1"

// Contains #defines for:
// WIFI_SSID, WIFI_PASSWORD, DATABASE_URL, DATABASE_SECRET
#include "creds.h"

#include <FirebaseESP32.h>
FirebaseData targetLuxStream;
FirebaseData motorEnabledStream;
FirebaseData firebaseIO;

#include "motor.h"
#define IN1 33
#define IN2 32
#define SENS1 27
#define SENS2 26
Motor motor(IN1, IN2, SENS1, SENS2);

#include "Adafruit_VEML7700.h"
Adafruit_VEML7700 lightSensor = Adafruit_VEML7700();


void setup() {
  Serial.begin(115200);
  initWifi(WIFI_SSID, WIFI_PASSWORD);
  initLightSensor();
  Serial.printf("\nFirebase Client v%s\n", FIREBASE_CLIENT_VERSION);
  initFirebase(DATABASE_URL, DATABASE_SECRET);
  initMotorEncoder();
  addListener(targetLuxStream, BLIND_NAME"/targetLux", targetLuxListener);
  addListener(motorEnabledStream, BLIND_NAME"/enabled", motorEnabledListener);
}


unsigned long timeElapsed = 0;
int prevEncoderVal = 0;
int prevLuxVal = 0;
int currentLux = 0;
int targetLux = -1;
void loop() {
  // Every 200ms:
  // - Update encoder value on Firebase if it has changed, so that in the event of
  //   a power failure the motor position can be restored when the ESP32 is powered back on.
  // - Update current lux value on Firebase if it has changed.
  // NOTE:
  // millis() overflows back to zero after approximately 50 days of continuous execution,
  // hence the last condition.
  if (millis() - timeElapsed > 200 || millis() - timeElapsed < 0) {
    if (motor.encoderVal != prevEncoderVal) {
      Firebase.setIntAsync(firebaseIO, BLIND_NAME"/encoderVal", motor.encoderVal);
      prevEncoderVal = motor.encoderVal;
    }
    currentLux = lightSensor.readLux();
    if (currentLux != prevLuxVal) {
      Firebase.setIntAsync(firebaseIO, BLIND_NAME"/currentLux", currentLux);
      prevLuxVal = currentLux;
    }
    timeElapsed = millis();
  }

  if (motor.enabled && targetLux != -1) {
    if (abs(targetLux - currentLux) <= 5) {
      motor.stop();
    } else if (targetLux > currentLux) {
      motor.clockwise();     // Raise blind
    } else {
      motor.antiClockwise(); // Lower blind
    }
  }
}


// Controls the motor based on new command received from Firebase
void targetLuxListener(StreamData data) {
  if (data.dataType() == "int") {
    targetLux = data.intData();
    Serial.printf("New target lux: %d lx\n", targetLux);
  }
}


void motorEnabledListener(StreamData data) {
  if (data.dataType() == "int") {
    motor.enabled = data.intData();
    Serial.printf("Motor %s\n", motor.enabled ? "enabled" : "disabled");
  }
}


// Sync encoder values with those stored on Firebase
// Register an interrupt to count encoder ticks
void initMotorEncoder() {
  Serial.print("Retrieving encoder values from Firebase... ");
  Firebase.getInt(firebaseIO, BLIND_NAME"/encoderMin", motor.encoderMin);
  Firebase.getInt(firebaseIO, BLIND_NAME"/encoderMax", motor.encoderMax);
  Firebase.getInt(firebaseIO, BLIND_NAME"/encoderVal", motor.encoderVal);
  attachInterrupt(digitalPinToInterrupt(SENS1), [](){ motor.encoderCallback(); }, RISING);
  Serial.println("done");
}


// Set up light sensor with gain and IT such that it can read lux values between 0-30,000lx
void initLightSensor() {
  Serial.print("Initialising light sensor... ");
  while (!lightSensor.begin());
  lightSensor.setGain(VEML7700_GAIN_1_4);
  lightSensor.setIntegrationTime(VEML7700_IT_50MS);
  Serial.println("done");
}
