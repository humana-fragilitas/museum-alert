//#include "mbed.h"

#include "Pins.h"
#include "PinSetup.h"
#include "SerialCom.h"
#include "UserPreferences.h"
#include "Sensor.h"
#include "WiFiManager.h"
#include "BLEManager.h"

#define DEFAULT_APP_STATE CONFIGURE_WIFI

enum State {
  INITIALIZE_SERIAL,
  CONFIGURE_WIFI,
  SET_PREFERENCES,
  SET_SSL_CERTIFICATE,
  CONNECT_TO_MQTT_BROKER,
  INITIALIZED
};

// Mutex mutex;
String ssid;
String pass;
bool debug = true;
State appState;
std::pair<UserSettings, bool> userPrefs;
UserPreferences userPreferences;
BLEManager bleManager(&onWiFiCredentials, &onTLSCertificate);
WiFiManager wiFiManager(&onWiFiEvent);
Sensor sensor;

const long configureWiFiInterval = 1000;
unsigned long previousWiFiIntervalMillis = 0;


void setup() {

  appState = DEFAULT_APP_STATE;

  if (debug) initializeSerial();

  pinSetup();

  switch (wiFiManager.connectToWiFi()) {

      case WL_CONNECTED:
        appState = SET_SSL_CERTIFICATE;
        break;
      case WL_CONNECT_FAILED:
        appState = CONFIGURE_WIFI;
        break;
      default:
        appState = CONFIGURE_WIFI;

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
    case SET_SSL_CERTIFICATE:
      // if already set, then appState = CONNECT_TO_MQTT_BROKER
      while(1) {
        Serial.println("\nSet SSL Certificate");
        delay(2000);
      }
      break;
    case CONNECT_TO_MQTT_BROKER:
      while(1) {
        Serial.println("\nConnect to MQTT Broker");
        delay(2000);
      }
      break;
    case INITIALIZED:
      while(1) {
        Serial.println("\nSensor check + BLE beacon");
        delay(2000);
      }
      break;

  }

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

void onWiFiEvent(WiFiEvent_t event) {

  switch (event) {

    case ARDUINO_EVENT_WIFI_READY: 
        Serial.printf("\nWiFi interface ready");
        break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
        Serial.printf("\nCompleted scan for access points");
        break;
    case ARDUINO_EVENT_WIFI_STA_START:
        Serial.printf("\nWiFi client started");
        break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
        Serial.printf("\nWiFi clients stopped");
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.printf("\nConnected to access point");
        digitalWrite(wiFiPin, HIGH);
        appState = SET_SSL_CERTIFICATE;
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.printf("\nDisconnected from WiFi access point");
        digitalWrite(wiFiPin, LOW);
        break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
        Serial.printf("\nAuthentication mode of access point has changed");
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.printf("\nObtained IP address: ");
        Serial.println(WiFi.localIP());
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        Serial.println("\nLost IP address and IP address is reset to 0");
        break;

  }

}

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

  //mutex.lock();
  //wiFiManager.connectToWiFi("Wind3 HUB - 0290C0", "73fdxdcc5x473dyz");
  // { "ssid": "Wind3 HUB - 0290C0", "pass":"73fdxdcc5x473dyz" }
  if (wiFiManager.connectToWiFi(ssid, pass) == WL_CONNECTED) {

      Serial.printf("\nConnected to WiFi network: %s", ssid.c_str());

  }
  //mutex.unlock();

}

void onTLSCertificate(String certificate) {

  Serial.printf("\nReceived TLS/SSL certificate: %s", certificate);

}



