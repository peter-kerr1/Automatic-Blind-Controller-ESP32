// The following global variables must be declared:
//   FirebaseData stream;
//   Motor motor(IN1, IN2);

#include <FirebaseESP32.h>
#include "motor.h"


// Must have Motor motor declared globally
void streamCallback(StreamData data) {
  if (data.dataType() == "string") {
    String command = data.stringData();
    Serial.printf("Command read: %s\n", command);
    if (command == "up") {
      motor.forward();
    } else if (command == "down") {
      motor.backward();
    } else {
      motor.stop();  // Stop on unexpected input as well as 'stop' command
    }
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
