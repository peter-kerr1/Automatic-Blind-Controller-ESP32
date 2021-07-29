#include <FirebaseESP32.h>

void streamCallback(StreamData data) {
  if (data.dataType() == "string") {
    Serial.printf("Data read: %s\n", data.stringData());
  }
}


void streamTimeoutCallback(bool timeout) {
  if (timeout) Serial.println("Stream timeout occurred, resuming...\n");
}


// FirebaseData stream; must be a global variable declared in main for this function to work.
void initFirebase(char* url, char* secret, char* path) {
  Firebase.reconnectWiFi(true);
  
  //Legacy authentication method because I'm lazy
  Firebase.begin(url, secret);
  
  if (!Firebase.beginStream(stream, path)) {
    Serial.printf("Failed to begin stream: %s\n\n", stream.errorReason().c_str());
  }
  
  Firebase.setStreamCallback(stream, streamCallback, streamTimeoutCallback);
}
