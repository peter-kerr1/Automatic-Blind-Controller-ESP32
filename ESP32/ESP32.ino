#define BLIND_NAME "blind1"

// Contains #defines for:
// WIFI_SSID, WIFI_PASSWORD, DATABASE_URL, DATABASE_SECRET
#include "creds.h"

#include <FirebaseESP32.h>
FirebaseData commandStream;
FirebaseData firebaseIO;

#include "motor.h"
#define IN1 33
#define IN2 32
#define SENS1 27
#define SENS2 26
Motor motor(IN1, IN2, SENS1, SENS2);

void setup() {
  Serial.begin(115200);
  initWifi(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("\nFirebase Client v%s\n", FIREBASE_CLIENT_VERSION);
  initFirebase(DATABASE_URL, DATABASE_SECRET);
  initMotorEncoder();
  addListener(commandStream, BLIND_NAME"/command", commandListener);
}


unsigned long timeElapsed = 0;
int prevEncoderVal = 0;
void loop() {
  if (motor.halt) {
    // Can't afford for this set call to fail, keep attempting until success
    while (!Firebase.setStringAsync(firebaseIO, BLIND_NAME"/command", "stop"));
    motor.halt = false;
  }
  // Update encoder value on Firebase every 200ms if it has changed, so that in the event of
  // a power failure the motor position can be restored when the ESP32 is powered back on.
  // millis() overflows back to zero after approximately 50 days of continuous execution,
  // hence the last condition.
  if (motor.encoderVal != prevEncoderVal && (millis() - timeElapsed > 200 || millis() - timeElapsed < 0)) {
    Firebase.setIntAsync(firebaseIO, BLIND_NAME"/encoderVal", motor.encoderVal);
    timeElapsed = millis();
    prevEncoderVal = motor.encoderVal;
  }
}


// Controls the motor based on new command received from Firebase
void commandListener(StreamData data) {
  if (data.dataType() == "string") {
    String command = data.stringData();
    Serial.printf("Command read: '%s'\n", command);
    if (command == "up") {
      motor.clockwise();
    } else if (command == "down") {
      motor.antiClockwise();
    } else {
      motor.stop();  // Stop on unexpected input as well as 'stop' command
    }
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
