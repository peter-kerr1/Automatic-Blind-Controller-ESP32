#include <FirebaseESP32.h>

void initFirebase(char* url, char* secret) {
  Serial.print("Connecting to Firebase...");
  Firebase.reconnectWiFi(true);
  Firebase.begin(url, secret); //Legacy authentication method because I'm lazy
  Serial.println("done");
}

void genericTimeoutCallback(bool timeout) {
  if (timeout) Serial.println("Stream timeout occurred, resuming...\n");
}

void addListener(FirebaseData &stream, char* path, FirebaseData::StreamEventCallback callbackFn) {
  Serial.printf("Attaching listener to '%s'...", path);
  if (!Firebase.beginStream(stream, path)) {
    Serial.printf("\nFailed to begin stream: %s\n\n", stream.errorReason());
  } else {
    Firebase.setStreamCallback(stream, callbackFn, genericTimeoutCallback);
    Serial.println("done");
  }
}
