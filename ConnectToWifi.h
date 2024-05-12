#include <WiFi.h>
#include "time.h"

const char* ssid     = "RUNI-Wireless";
const char* password = "";

void connectToWifi(){
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}
