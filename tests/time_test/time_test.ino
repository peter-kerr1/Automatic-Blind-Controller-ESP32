#include <WiFi.h>
#include "time.h"
#include "creds.h"

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

  configTime(11 * 3600, 3600, "pool.ntp.org");
  
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  } else {
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    Serial.println(timeinfo.tm_hour);
  }
}

void loop() {
  
}
