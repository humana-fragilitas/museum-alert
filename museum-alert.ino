#include <ArduinoJson.h>

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
}

State appState = STARTED;

void setup() {

  UserPreferences userPreferences;
  BLEManager bleManager;
  WiFiManager wiFiManager;
  Sensor sensor;

  initializeSerial();
  pinSetup();

  if (!userPreferences.getPreferences()) {
    bleManager.initializeBLE();
  }

  wiFiManager.listNetworks();
  wiFiManager.connectToWiFi();

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


