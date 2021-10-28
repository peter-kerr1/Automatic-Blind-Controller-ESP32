#include <FirebaseESP32.h>

void initFirebase(char* url, char* secret) {
  Firebase.reconnectWiFi(true);
  Firebase.begin(url, secret); //Legacy authentication method because I'm lazy
}

void genericTimeoutCallback(bool timeout) {
  if (timeout) Serial.println("Stream timeout occurred, resuming...\n");
}

void addListener(FirebaseData &stream, char* path, FirebaseData::StreamEventCallback callbackFn) {
  if (!Firebase.beginStream(stream, path)) {
    Serial.printf("Failed to begin stream: %s\n\n", stream.errorReason());
  } else {
    Firebase.setStreamCallback(stream, callbackFn, genericTimeoutCallback);
  } 
}
