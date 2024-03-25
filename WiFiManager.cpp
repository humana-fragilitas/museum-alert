#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#include "Pins.h"
#include "WiFiManager.h"

WiFiManager::WiFiManager(void(*onWiFiConnectionSuccess)(void), void(*onWiFiConnectionError)(void)) {

  _onWiFiConnectionSuccess = onWiFiConnectionSuccess;
  _onWiFiConnectionError = onWiFiConnectionError;

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

void WiFiManager::connectToWiFi(String ssid, String pass) {

  wl_status_t status;

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(false, true);
  digitalWrite(wiFiPin, LOW);

  WiFi.begin(ssid, pass);

  while((status = WiFi.status()) != WL_CONNECTED) {

      if (status == WL_CONNECT_FAILED  ||
          status == WL_CONNECTION_LOST ||
          status == WL_DISCONNECTED) {

        _onWiFiConnectionError();
        return;
        break;

      }

  }

  digitalWrite(wiFiPin, HIGH);
  Serial.printf("\nConnected to WiFi network: %s", ssid.c_str());
  Serial.printf("\nLocal ESP32 IP: %s", WiFi.localIP().toString());

  _onWiFiConnectionSuccess();

}

wl_status_t WiFiManager::connectToWiFi(void) {

  return WiFi.begin();

}

bool WiFiManager::eraseConfiguration(void) {

  return WiFi.eraseAP();

}

void WiFiManager::disconnect() {

  WiFi.disconnect(false, true);

  digitalWrite(wiFiPin, LOW);

  Serial.println("\nDisconnected from WiFi network");
  
}