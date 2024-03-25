
#include "Pins.h"
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

String ssid;
String pass;
bool debug = true;
State appState;
std::pair<UserSettings, bool> userPrefs;
UserPreferences userPreferences;
BLEManager bleManager(&onWiFiCredentials, &onTLSCertificate);
WiFiManager wiFiManager(&onWiFiConnectionSuccess, &onWiFiConnectionError);
Sensor sensor;

const long configureWiFiInterval = 5000;
unsigned long previousWiFiIntervalMillis = 0;


void setup() {

  if(debug) initializeSerial();

  pinSetup();

  if(!wiFiManager.connectToWiFi()) {

    wiFiManager.eraseConfiguration();

    appState = CONFIGURE_WIFI;

  } else {

    appState = SET_SSL_CERTIFICATE;

  }

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
      // { "ssid": "Wind3 HUB - 0290C0", "pass":"73fdxdcc5x473dyz" }
      wiFiManager.connectToWiFi(userPrefs.first.ssid, userPrefs.first.pass);
      break;
    case SET_SSL_CERTIFICATE:
      while(1) {
        Serial.println("\nSet SSL Certificate");
        delay(2000);
      }
      break;

  }

  //bleManager.configureViaBLE();
  //sensor.detect();

}

/******************************************************************************
 * LOOP FUNCTIONS                                                             *
 *****************************************************************************/

void configureWiFi() {

  StaticJsonDocument<4096> doc;
  JsonArray arr = doc.to<JsonArray>();
  char json[4096];
  wiFiManager.listNetworks(&arr);
  serializeJson(doc, json);
  bleManager.configureWiFi(json);
  digitalWrite(wiFiPin, !digitalRead(wiFiPin));

}

/******************************************************************************
 * CALLBACK FUNCTIONS                                                         *
 *****************************************************************************/

void onWiFiCredentials(String credentials) {

  UserSettings userSettings;

  Serial.printf("\nReceived WiFi credentials: %s", credentials.c_str());

  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, credentials);

  if (error) {
    Serial.println("Failed to deserialize WiFi credentials json: ");
    Serial.println(error.c_str());
    return;
  }

  ssid = doc["ssid"].as<String>();
  pass = doc["pass"].as<String>();

  appState = CONNECT_TO_WIFI;

}

void onTLSCertificate(String certificate) {

  Serial.printf("\nReceived TLS/SSL certificate: %s", certificate);

}

void onWiFiConnectionSuccess(void) {

  appState = SET_SSL_CERTIFICATE;

}

void onWiFiConnectionError(void) {

  Serial.println("\nCan't connect to WiFi");

  userPreferences.deletePreferences();

  appState = CONFIGURE_WIFI;

}



