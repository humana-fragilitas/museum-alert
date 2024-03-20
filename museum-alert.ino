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
BLEManager bleManager(&onWiFiCredentials, &onTLSCertificate);
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

}

void loop() {

  switch(appState) {

    case START:
      pinSetup();
      // fall through
    case BROADCAST_WIFI_NETWORKS:
      broadcastWiFiNetworksWithDelay(1000);
      break;
    case CONNECT_TO_WIFI:
      wiFiManager.connectToWiFi("Wind3 HUB - 0290C0", "73fdxdcc5x473dyz");
      break;

  }

  bleManager.configureViaBLE();
  sensor.detect();

}

/******************************************************************************
 * LOOP FUNCTIONS                                                             *
 *****************************************************************************/

void broadcastWiFiNetworksWithDelay(short milliseconds) {

      JsonDocument doc;
      char json[4096];
      wiFiManager.listNetworks(&doc);
      serializeJson(doc, json);
      bleManager.broadCastWiFiSsids(json);
      delay(milliseconds);

}

/******************************************************************************
 * CALLBACK FUNCTIONS                                                         *
 *****************************************************************************/

void onWiFiCredentials(std::pair<UserSettings, bool> credentials) {

  Serial.printf("\nReceived WiFi credentials; ssid: %s password: %s", credentials.first.ssid, credentials.first.pass);

  //userPrefs = credentials.first;

  appState = CONNECT_TO_WIFI;

}

void onTLSCertificate(char certificate[]) {

  Serial.printf("\nReceived TLS/SSL certificate: %s", certificate);

}

void onWiFiConnectionSuccess(void) {



}

void onWiFiConnectionError(void) {



}



