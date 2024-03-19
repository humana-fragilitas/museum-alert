#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>


#include "Pins.h"
#include "WiFiManager.h"

void WiFiManager::listNetworks(JsonDocument* doc) {

  byte numSsid;

  Serial.println("\nScanning WiFi networks");

    while(!(numSsid = WiFi.scanNetworks())){
        Serial.print(".");
        delay(100);
    }

  // print the list of networks seen:

  Serial.printf("\nNumber of available networks: %d", numSsid);

  // print the network number and name for each network found:

  for (int i = 0; i < numSsid; ++i) {

    Serial.printf("\n%d) %s | signal: %d dbm | encryption: %d",
      i, WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));

    Serial.println(WiFi.SSID(i));

    doc[i]["ssid"] = WiFi.SSID(i);
    doc[i]["rssi"] = WiFi.RSSI(i);
    doc[i]["encryptionType"] = WiFi.encryptionType(i);

  }

}

void WiFiManager::connectToWiFi(String ssid, String pass) {

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    digitalWrite(wiFiPin, LOW);
    delay(100);
  
    WiFi.begin(ssid, pass);
    Serial.println("\nConnecting to WiFi");

    // https://www.arduino.cc/reference/en/libraries/wifi/wifi.status/
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
    }

    digitalWrite(wiFiPin, HIGH);
    Serial.printf("\nConnected to WiFi network: %s", ssid);
    Serial.printf("\nLocal ESP32 IP: %s", WiFi.localIP().toString());

}