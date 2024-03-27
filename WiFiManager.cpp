#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include "WiFiManager.h"

WiFiManager::WiFiManager(void(*onWiFiEvent)(WiFiEvent_t)) {

  WiFi.mode(WIFI_STA);
  WiFi.onEvent(onWiFiEvent);

}

void WiFiManager::listNetworks(JsonArray* doc) {

  WiFi.setAutoReconnect(false);

  byte numSsid;

  Serial.println("\nScanning WiFi networks");

  while(!(numSsid = WiFi.scanNetworks())){
      //Serial.print(".");
      //delay(100);
  }

  // print the list of networks seen:

  Serial.printf("\nNumber of available networks: %d", numSsid);

  // print the network number and name for each network found:

  for (int i = 0; i < numSsid; ++i) {

    Serial.printf("\n%u) %s | signal: %d dbm | encryption: %d ",
      i, WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));

    Serial.println(WiFi.SSID(i));

    JsonObject wiFiEntry = doc->createNestedObject();
    wiFiEntry["ssid"] = WiFi.SSID(i);
    wiFiEntry["rssi"] = WiFi.RSSI(i);
    wiFiEntry["encryptionType"] = WiFi.encryptionType(i);

  }

}

uint8_t WiFiManager::connectToWiFi(String ssid, String pass) {

  uint8_t status;

  WiFi.begin(ssid, pass);

  status = WiFi.waitForConnectResult();

  return status;

}

uint8_t WiFiManager::connectToWiFi(void) {

  uint8_t status;

  WiFi.begin();

  status = WiFi.waitForConnectResult();

  return status;

}

bool WiFiManager::eraseConfiguration(void) {

  return WiFi.eraseAP();

}

void WiFiManager::disconnect() {

  WiFi.disconnect(false, true);

  Serial.println("\nDisconnected from WiFi network");
  
}