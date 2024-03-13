#include "PinSetup.h"
#include "SerialCom.h"
#include "UserPreferences.h"
#include "Sensor.h"
#include "WiFiManager.h"
#include "BLEManager.h"

enum State {
  STARTED,
  SERIAL_READY,
  PINS_SETUP,
  NO_PREFERENCES_SET,
  PREFERENCES_SET,
  BROADCAST_WIFI_NETWORKS,
  CONNECT_TO_WIFI
};

State appState = STARTED;

UserPreferences userPreferences;
BLEManager bleManager;
WiFiManager wiFiManager;
Sensor sensor;

void setup() {

  initializeSerial();
  pinSetup();

  if (!userPreferences.getPreferences().second) {
    bleManager.initializeBLE();
  }

  wiFiManager.listNetworks();
  wiFiManager.connectToWiFi("Wind3 HUB - 0290C0", "73fdxdcc5x473dyz");

}

void loop() {

  switch(appState) {
    case NO_PREFERENCES_SET:

    break;
    case PREFERENCES_SET:

    break;

  }

  bleManager.configureViaBLE();
  sensor.detect();

}


