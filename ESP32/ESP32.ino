#define BLIND_NAME "blind1"

// Contains #defines for:
// WIFI_SSID, WIFI_PASSWORD, DATABASE_URL, DATABASE_SECRET
#include "creds.h"

// Uses v3.10.5 of Firebase ESP32 Client by Mobizt
#include <FirebaseESP32.h>
FirebaseData firebaseIO;
FirebaseData settingStream;
String settings[3] = {"/command", "/lowerTime", "/raiseTime"};

#include "motor.h"
#define PWM 25
#define IN1 33
#define IN2 32
#define SENS1 27
#define SENS2 26
Motor motor(PWM, IN1, IN2, SENS1, SENS2);

// Uses v1.1.1 of Adafruit VEML7700 Library by Adafruit
#include "Adafruit_VEML7700.h"
Adafruit_VEML7700 lightSensor = Adafruit_VEML7700();

// https://www.reddit.com/r/esp32/comments/jx09hj/hot_to_obtain_local_time_zone_offset_including/
#include "time.h"
#define TIMEZONE_SYD "AEST-10AEDT,M10.1.0,M4.1.0/3"

void setup() {
  Serial.begin(115200);
  initWifi(WIFI_SSID, WIFI_PASSWORD);
  configTzTime(TIMEZONE_SYD, "pool.ntp.org");
  initLightSensor();
  initFirebase(DATABASE_URL, DATABASE_SECRET);
  initMotorEncoder();
  addMultiPathListener(settingStream, BLIND_NAME"/settings", settings, settingListener);
}



unsigned long sensorTimer = 0;
unsigned long blindTimer = 0;
int prevEncoderVal = -1;
int prevLux = 0;
int currentLux = 0;
int raiseTime = -1;
int lowerTime = -1;

void loop() {
  if (motor.halt) {
    // Can't afford for this set call to fail, keep attempting until success
    while (!Firebase.setStringAsync(firebaseIO, BLIND_NAME"/settings/command", "stop"));
    motor.halt = false;
  }
  
  // Every 200ms:
  // - Update encoder value on Firebase if it has changed, so that in the event of
  //   a power failure the motor position can be restored when the ESP32 is powered back on.
  // - Update current lux value on Firebase if it has changed.
  // NOTE:
  // millis() overflows back to zero after approximately 50 days of continuous execution,
  // hence the last condition.
  if (millis() - sensorTimer >= 200 || millis() - sensorTimer < 0) {
    if (motor.encoderVal != prevEncoderVal) {
      Firebase.setIntAsync(firebaseIO, BLIND_NAME"/encoderVal", motor.encoderVal);
      prevEncoderVal = motor.encoderVal;
    }
    currentLux = lightSensor.readLux();
    if (currentLux != prevLux) {
      Firebase.setIntAsync(firebaseIO, BLIND_NAME"/currentLux", currentLux);
      prevLux = currentLux;
    }
    sensorTimer = millis();
  }
  
  // Every 30 seconds, check to see if it's time to raise or lower the blinds.
  // If it is, we update the command on Firebase so that the GUI syncs up with what the blind is doing.
  if (millis() - blindTimer >= 30000 || millis() - blindTimer < 0) {
    int currentTime = getTime();
    if (currentTime == raiseTime) {
      while (!Firebase.setStringAsync(firebaseIO, BLIND_NAME"/settings/command", "up"));
    } else if (currentTime == lowerTime) {
      while (!Firebase.setStringAsync(firebaseIO, BLIND_NAME"/settings/command", "down"));
    }
    blindTimer = millis();
  }
}



// Returns the current time as an integer in 24-hour format (eg: 6:31pm -> 1831)
int getTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("[ERROR] Failed to obtain current time");
    return 0;
  } else {
    return timeinfo.tm_hour * 100 + timeinfo.tm_min;
  }
}

// Controls the motor based on new command received from Firebase
void executeCommand(String command) {
  if (command == "up") {
    motor.clockwise();
  } else if (command == "down") {
    motor.antiClockwise();
  } else {
    motor.stop();  // Stop on unexpected input as well as 'stop' command
  }
}

// 
void settingListener(MultiPathStreamData node) {
  for (String setting : settings) {
    if (node.get(setting)) {
      Serial.printf("New %s received: '%s'\n", &setting[1], node.value);
      if (setting == "/command") {
        executeCommand(node.value);
      } else if (setting == "/lowerTime") {
        lowerTime = node.value.toInt();
      } else if (setting == "/raiseTime") {
        raiseTime = node.value.toInt();
      }
    }
  }
}

// Sync encoder values with those stored on Firebase
// Register an interrupt to count encoder ticks
void initMotorEncoder() {
  Serial.print("Retrieving encoder values from Firebase... ");
  while(!Firebase.getInt(firebaseIO, BLIND_NAME"/encoderMin", motor.encoderMin));
  while(!Firebase.getInt(firebaseIO, BLIND_NAME"/encoderMax", motor.encoderMax));
  while(!Firebase.getInt(firebaseIO, BLIND_NAME"/encoderVal", motor.encoderVal));
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
