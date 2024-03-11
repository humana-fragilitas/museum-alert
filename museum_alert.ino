#include <ArduinoJson.h>

#include "PinSetup.h"
#include "SerialCom.h"
#include "UserPreferences.h"
#include "Sensor.h"
#include "WiFiManager.h"

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

  initializeSerial();
  pinSetup();

  if (getPreferences() == null) {
    BLEManager.initializeBLE();
  }

  listNetworks();
  connectToWiFi();

}

void loop() {

  switch(appState) {
    case NO_PREFERENCES_SET:

    break;
    case PREFERENCES_SET:

    break;

  }

  BLEManager.configureViaBLE();
  initializeSensor();

}


