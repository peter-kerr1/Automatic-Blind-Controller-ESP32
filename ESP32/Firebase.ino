#include <FirebaseESP32.h>

void initFirebase(char* url, char* secret) {
  Serial.printf("\nFirebase Client v%s\n", FIREBASE_CLIENT_VERSION);
  Serial.print("Connecting to Firebase... ");
  Firebase.reconnectWiFi(true);
  Firebase.begin(url, secret); //Legacy authentication method because I'm lazy
  Serial.println("done");
}

void genericTimeoutCallback(bool timeout) {
  if (timeout) Serial.println("Stream timeout occurred, resuming...\n");
}

void addMultiPathListener(FirebaseData &stream, String parentPath, String* childPaths, FirebaseData::MultiPathStreamEventCallback callbackFn) {
  Serial.printf("Attaching listener to '%s'... ", parentPath.c_str());
  if (!Firebase.beginMultiPathStream(stream, parentPath, childPaths, sizeof(childPaths)/sizeof(childPaths[0]))) {
    Serial.printf("\nFailed to begin stream: %s\n\n", stream.errorReason());
  } else {
    Firebase.setMultiPathStreamCallback(stream, callbackFn, genericTimeoutCallback);
    Serial.println("done");
  }
}
