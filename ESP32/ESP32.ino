// Contains #defines for:
// WIFI_SSID, WIFI_PASSWORD, DATABASE_URL, DATABASE_SECRET
#include "creds.h"

#include <FirebaseESP32.h>
FirebaseData stream;

void setup() {
  Serial.begin(115200);
  initWiFi(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  initFirebase(DATABASE_URL, DATABASE_SECRET, "/command");
}

void loop() {
}
