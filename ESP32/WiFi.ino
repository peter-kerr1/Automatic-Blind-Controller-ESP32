#include <WiFi.h>

void initWifi(char* ssid, char* password) {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(300);
  }
  Serial.printf("\nConnected with IP: ");
  Serial.println(WiFi.localIP());
}
