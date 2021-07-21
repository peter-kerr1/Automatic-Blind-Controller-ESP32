#include <WiFi.h>
#include <FirebaseESP32.h>

// Contains #defines for:
// WIFI_SSID, WIFI_PASSWORD, DATABASE_URL, DATABASE_SECRET
#include "creds.h"

FirebaseData fbdo;

unsigned long dataMillis = 0;
int count = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  
  Firebase.reconnectWiFi(true);
  
  //Legacy authentication method because I'm lazy
  Firebase.begin(DATABASE_URL, DATABASE_SECRET);
}

void loop() {
  if (millis() - dataMillis > 100) {
      dataMillis = millis();
      Serial.printf("Set int... %s\n", Firebase.setIntAsync(fbdo, "/test", count++) ? "ok" : fbdo.errorReason().c_str());
  }
}
