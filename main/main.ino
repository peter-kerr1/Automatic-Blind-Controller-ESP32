#include <WiFi.h>
#include <FirebaseESP32.h>
#include "addons/RTDBHelper.h"

// Contains #defines for:
// WIFI_SSID, WIFI_PASSWORD, DATABASE_URL, DATABASE_SECRET
#include "creds.h"

FirebaseData stream;
//FirebaseData fbdo;

unsigned long dataMillis = 0;
int count = 0;

void streamCallback(StreamData data)
{
  Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\n\n",
                data.streamPath().c_str(),
                data.dataPath().c_str(),
                data.dataType().c_str(),
                data.eventType().c_str());
  printResult(data); //see addons/RTDBHelper.h
  Serial.println();
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
    Serial.println("stream timeout, resuming...\n");
}

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

  Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);

  // Errors initially - check this out?
  if (!Firebase.beginStream(stream, "/test"))
    Serial.printf("stream begin error, %s\n\n", stream.errorReason().c_str());
}

void loop() {
}
