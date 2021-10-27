// Contains #defines for:
// WIFI_SSID, WIFI_PASSWORD, DATABASE_URL, DATABASE_SECRET
#include "creds.h"

#include <FirebaseESP32.h>
FirebaseData stream;

#include "motor.h"
#define IN1 33
#define IN2 32
Motor motor(IN1, IN2);

void setup() {
  Serial.begin(115200);
  initWifi(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  initFirebase(DATABASE_URL, DATABASE_SECRET, "/command");
}

void loop() {
}

//Idea: store the encoder value in the Motor class, and have an update method (continually called in loop) that stores the value and also compares it against some pre-defined limits (this will require initial calibration of some kind.) -> see below
//      When a limit is reached, stop the motor and change a 'limit' variable to -1,0,1 to reflect whether the top or bottom limit has been reached. The forward() and backward() functions can then check this limit variable to see if they should run or not.

//      Calibration: technically, calibration only needs to be performed once per blind, since the calibration values can be stored on Firebase. This will also require the current value of the encoder to continually be pushed to the server,
//                   so that on reboot the esp32 knows where it is relative to the limits. 
