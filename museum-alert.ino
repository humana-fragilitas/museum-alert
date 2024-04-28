//#include "mbed.h"

#include "Pins.h"
#include "PinSetup.h"
#include "SerialCom.h"
#include "UserPreferences.h"
#include "Sensor.h"
#include "WiFiManager.h"
#include "BLEManager.h"

#include <WiFi.h>

enum State {
  INITIALIZE_SERIAL,
  CONFIGURE_WIFI,
  CONNECT_TO_WIFI,
  SET_PREFERENCES,
  SET_SSL_CERTIFICATE,
  CONNECT_TO_MQTT_BROKER,
  INITIALIZED
};

// Mutex mutex;
TaskHandle_t ledIndicatorsTask;
String ssid;
String pass;
bool debug = true;
bool wiFiLedStatus = false;
State appState;
std::pair<UserSettings, bool> userPrefs;
UserPreferences userPreferences;
BLEManager bleManager(&onWiFiCredentials, &onTLSCertificate);
WiFiManager wiFiManager(&onWiFiEvent);
Sensor sensor;

unsigned const int configureWiFiInterval = 4000;
unsigned int previousWiFiInterval = 0;

unsigned const int resetButtonInterval = 4000;
unsigned int previousResetButtonInterval = 0;

void setup() {

  if (debug) initializeSerial();

  pinSetup();

  Serial.println("\nBegin delay: 20 sec.");
  delay(20000);
  Serial.println("\nDelay end.");

  attachInterrupt(digitalPinToInterrupt(resetButtonPin), onResetButtonISR, CHANGE);

  xTaskCreatePinnedToCore(
    ledIndicators,
    "LedIndicators",
    1024,
    NULL,
    0,
    &ledIndicatorsTask,
    0
  );

  appState = CONNECT_TO_WIFI;

  //BaseType_t coreID = xPortGetCoreID();
  //Serial.print("setup() is running on core ");
  //Serial.println(coreID);

}

void loop() {

  //BaseType_t coreID = xPortGetCoreID();
  //Serial.print("loop() is running on core ");
  //Serial.println(coreID);

  unsigned long currentMillis = millis();

  switch(appState) {

    case CONNECT_TO_WIFI:
      if (wiFiManager.connectToWiFi() == WL_CONNECTED) {
        appState = SET_SSL_CERTIFICATE;
      } else {
        bleManager.initializeBLEConfigurationService();
        appState = CONFIGURE_WIFI;
      }
      break;
    case CONFIGURE_WIFI:
      if (currentMillis - previousWiFiInterval >= configureWiFiInterval) {
        configureWiFi();
        Serial.printf("Millis: %d", currentMillis - previousWiFiInterval);
        previousWiFiInterval = currentMillis;
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
        appState = CONNECT_TO_WIFI;
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.printf("\nConnected to access point");
        appState = SET_SSL_CERTIFICATE;
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.printf("\nDisconnected from WiFi access point");
        appState = CONNECT_TO_WIFI;
        break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
        Serial.printf("\nAuthentication mode of access point has changed");
        appState = CONNECT_TO_WIFI;
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.printf("\nObtained IP address: ");
        Serial.println(WiFi.localIP());
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        Serial.println("\nLost IP address and IP address is reset to 0");
        appState = CONNECT_TO_WIFI;
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
  // { "ssid": "Pixel_9824", "pass":"qyqijczyz2p37xz" }
  if (wiFiManager.connectToWiFi(ssid, pass) == WL_CONNECTED) {

      Serial.printf("\nConnected to WiFi network: %s", ssid.c_str());

  }
  //mutex.unlock();

}

void onTLSCertificate(String certificate) {

  Serial.printf("\nReceived TLS/SSL certificate: %s", certificate);

}

void onResetButtonISR(void) {

  unsigned long currentMillis = millis();

  if (digitalRead(resetButtonPin)) {

    previousResetButtonInterval = currentMillis;

  } else {

    if (currentMillis - previousResetButtonInterval >= resetButtonInterval) {

      Serial.println("Erasing AP settings and rebooting...");
      WiFi.eraseAP();
      //esp_wifi_start();
      // Note: first restart after serial flashing causes puts the board in boot mode:(1,7) (purple led)
      // https://github.com/esp8266/Arduino/issues/1722
      // https://github.com/esp8266/Arduino/issues/1017
      // https://github.com/esp8266/Arduino/issues/1722#issuecomment-321818357
      ESP.restart();

    }

  }

}

/******************************************************************************
 * MULTITHREADING FUNCTIONS                                                   *
 *****************************************************************************/

void ledIndicators(void *parameter) {

  unsigned long previousLedBlinkInterval = 0;
  const unsigned long ledBlinkInterval = 500;

  unsigned long previousLedBlinkInterval2 = 0;
  const unsigned long ledBlinkInterval2 = 250;

  for(;;) {

    unsigned long currentMillis = millis();

    switch(appState) {

      case CONNECT_TO_WIFI:
        if (currentMillis - previousLedBlinkInterval >= ledBlinkInterval) {
          digitalWrite(wiFiPin, !digitalRead(wiFiPin));
          previousLedBlinkInterval = currentMillis;
        }
        break;
      case CONFIGURE_WIFI:
        if (currentMillis - previousLedBlinkInterval2 >= ledBlinkInterval2) {
          digitalWrite(wiFiPin, !digitalRead(wiFiPin));
          previousLedBlinkInterval2 = currentMillis;
        }
        break;

    }
    
  }

}

