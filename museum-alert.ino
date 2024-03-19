#include "PinSetup.h"
#include "SerialCom.h"
#include "UserPreferences.h"
#include "Sensor.h"
#include "WiFiManager.h"
#include "BLEManager.h"

enum State {
  START,
  INITIALIZE_SERIAL,
  BROADCAST_WIFI_NETWORKS,
  SET_PREFERENCES,
  CONNECT_TO_WIFI,
  SET_SSL_CERTIFICATE,
  CONNECT_TO_MQTT_BROKER
};

bool debug = true;
std::pair<UserSettings, bool> userPrefs;
UserPreferences userPreferences;
BLEManager bleManager;
WiFiManager wiFiManager;
Sensor sensor;

State appState = START;

void setup() {
  
  pinSetup();

  if(debug) {
    initializeSerial();
  }

  userPrefs = userPreferences.getPreferences();
  
  if (!userPrefs.second) {
    bleManager.initializeBLEConfigurationService();
    appState = BROADCAST_WIFI_NETWORKS;
  } else {
    appState = CONNECT_TO_WIFI;
  }

  //wiFiManager.connectToWiFi("Wind3 HUB - 0290C0", "73fdxdcc5x473dyz");

}

void loop() {

  switch(appState) {

    case START:
      pinSetup();
    case BROADCAST_WIFI_NETWORKS:
      JsonDocument doc;
      char json[4096];
      wiFiManager.listNetworks(&doc);
      serializeJson(doc, json);
      bleManager.broadCastWiFiSsids(json);
      delay(1000);
      break;

  }

  bleManager.configureViaBLE();
  sensor.detect();

}


