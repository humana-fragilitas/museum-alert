#include "PinSetup.h"
#include "SerialCom.h"
#include "UserPreferences.h"
#include "Sensor.h"
#include "WiFiManager.h"
#include "BLEManager.h"

enum State {
  INITIALIZE_SERIAL,
  CONFIGURE_WIFI,
  SET_PREFERENCES,
  CONNECT_TO_WIFI,
  SET_SSL_CERTIFICATE,
  CONNECT_TO_MQTT_BROKER
};

bool debug = true;
State appState;
String ssid;
String pass;
std::pair<UserSettings, bool> userPrefs;
UserPreferences userPreferences;
BLEManager bleManager(&onWiFiCredentials, &onTLSCertificate);
WiFiManager wiFiManager;
Sensor sensor;

const long configureWiFiInterval = 1000;
unsigned long previousWiFiIntervalMillis = 0;

void setup() {
  
  pinSetup();

  if(debug) {
    initializeSerial();
  }

  //userPrefs = userPreferences.getPreferences();
 /* 
  if (!userPrefs.second) {
    bleManager.initializeBLEConfigurationService();
    appState = BROADCAST_WIFI_NETWORKS;
  } else {
    appState = CONNECT_TO_WIFI;
  }*/

  bleManager.initializeBLEConfigurationService();

  appState = CONFIGURE_WIFI;

}

void loop() {

  unsigned long currentMillis = millis();

  switch(appState) {

    case CONFIGURE_WIFI:
      if (currentMillis - previousWiFiIntervalMillis >= configureWiFiInterval) {
        configureWiFi();
        previousWiFiIntervalMillis = currentMillis;
      }
      break;
    case CONNECT_TO_WIFI:
      //wiFiManager.connectToWiFi("Wind3 HUB - 0290C0", "73fdxdcc5x473dyz");
      wiFiManager.connectToWiFi(ssid, pass);
      break;

  }

  //bleManager.configureViaBLE();
  //sensor.detect();

}

/******************************************************************************
 * LOOP FUNCTIONS                                                             *
 *****************************************************************************/

void configureWiFi() {

      JsonDocument doc;
      char json[4096];
      wiFiManager.listNetworks(&doc);
      serializeJson(doc, json);
      bleManager.configureWiFi(json);

}

/******************************************************************************
 * CALLBACK FUNCTIONS                                                         *
 *****************************************************************************/

void onWiFiCredentials(String credentials) {

  Serial.printf("\nReceived WiFi credentials: %s", credentials);

  JsonDocument doc;

  deserializeJson(doc, credentials);

  ssid = doc["ssid"].as<String>();
  pass = doc["pass"].as<String>();

  appState = CONNECT_TO_WIFI;

}

void onTLSCertificate(String certificate) {

  Serial.printf("\nReceived TLS/SSL certificate: %s", certificate);

}

void onWiFiConnectionSuccess(void) {



}

void onWiFiConnectionError(void) {



}



