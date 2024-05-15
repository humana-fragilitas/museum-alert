//#include "mbed.h"

#include <mqtt_client.h>

#include "Pins.h"
#include "PinSetup.h"
#include "SerialCom.h"
#include "UserPreferences.h"
#include "Sensor.h"
#include "WiFiManager.h"
#include "BLEManager.h"
#include "MQTTClient.h"

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
bool wiFiLedStatus = false;
State appState;
std::pair<UserSettings, bool> userPrefs;
UserPreferences userPreferences;
MQTTClient mqttClient(&onMqttEvent, &onMqttMessage);
BLEManager bleManager(&onWiFiCredentials, &onTLSCertificate);
WiFiManager wiFiManager(&onWiFiEvent);
Sensor sensor;

unsigned const int configureWiFiInterval = 4000;
unsigned int previousWiFiInterval = 0;

unsigned const int resetButtonInterval = 4000;
unsigned int previousResetButtonInterval = 0;

int brokerSetUp = 0;
esp_mqtt_client_handle_t mqttClientHandle;

void setup() {

  initializeSerial();

  pinSetup();

  forceDelay();

  initializeUI();

  appState = (wiFiManager.connectToWiFi() != WL_CONNECTED) ?
    CONFIGURE_WIFI : SET_SSL_CERTIFICATE;

  if (appState == CONFIGURE_WIFI) bleManager.initializeBLEConfigurationService();

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
      //err = esp_tls_set_global_ca_store(DSTroot_CA, sizeof(DSTroot_CA));
	    //Serial.printf("\nCA store set. Error = %d %s", err, esp_err_to_name(err));
      appState = CONNECT_TO_MQTT_BROKER;
      break;

    case CONNECT_TO_MQTT_BROKER:
      if(!brokerSetUp) {
        brokerSetUp = 1;
        Serial.println("Connect to MQTT Broker");
        mqttClientHandle = mqttClient.connect().first;
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
 * SETUP FUNCTIONS                                                             *
 *****************************************************************************/

void forceDelay() {

  Serial.println("Begin delay: 20 sec.");
  delay(20000);
  Serial.println("Delay end.");

}

void initializeUI() {

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
        Serial.println("WiFi interface ready");
        break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
        Serial.println("Completed scan for access points");
        break;
    case ARDUINO_EVENT_WIFI_STA_START:
        Serial.println("WiFi client started");
        break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
        Serial.println("WiFi clients stopped");
        appState = CONNECT_TO_WIFI;
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.println("Connected to access point");
        appState = SET_SSL_CERTIFICATE;
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("Disconnected from WiFi access point");
        appState = CONNECT_TO_WIFI;
        break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
        Serial.println("Authentication mode of access point has changed");
        appState = CONNECT_TO_WIFI;
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.printf("Obtained IP address: %s", (String)WiFi.localIP());
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        Serial.println("Lost IP address and IP address is reset to 0");
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

void onMqttMessage(char* topic, byte* payload, unsigned int length)
{
  Serial.println("Received [");
  Serial.println(topic);
  Serial.println("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
}

esp_err_t onMqttEvent(esp_mqtt_event_handle_t event)
{

  static char incoming_data[128];

  switch (event->event_id)
  {
    int i, r;

    case MQTT_EVENT_ERROR:
      Serial.println("MQTT event MQTT_EVENT_ERROR");
      break;
    case MQTT_EVENT_CONNECTED:
      Serial.println("MQTT event MQTT_EVENT_CONNECTED");

      r = esp_mqtt_client_subscribe(mqttClientHandle, AZ_IOT_HUB_CLIENT_C2D_SUBSCRIBE_TOPIC, 1);
      if (r == -1)
      {
        Serial.println("Could not subscribe for cloud-to-device messages.");
      }
      else
      {
        Serial.println("Subscribed for cloud-to-device messages; message id:" + String(r));
      }

      break;
    case MQTT_EVENT_DISCONNECTED:
      Serial.println("MQTT event MQTT_EVENT_DISCONNECTED");
      break;
    case MQTT_EVENT_SUBSCRIBED:
      Serial.println("MQTT event MQTT_EVENT_SUBSCRIBED");
      break;
    case MQTT_EVENT_UNSUBSCRIBED:
      Serial.println("MQTT event MQTT_EVENT_UNSUBSCRIBED");
      break;
    case MQTT_EVENT_PUBLISHED:
      Serial.println("MQTT event MQTT_EVENT_PUBLISHED");
      break;
    case MQTT_EVENT_DATA:
      Serial.println("MQTT event MQTT_EVENT_DATA");

      for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->topic_len; i++)
      {
        incoming_data[i] = event->topic[i];
      }
      incoming_data[i] = '\0';
      Serial.println("Topic: " + String(incoming_data));

      for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->data_len; i++)
      {
        incoming_data[i] = event->data[i];
      }
      incoming_data[i] = '\0';
      Serial.println("Data: " + String(incoming_data));

      break;
    case MQTT_EVENT_BEFORE_CONNECT:
      Serial.println("MQTT event MQTT_EVENT_BEFORE_CONNECT");
      break;
    default:
      Serial.println("MQTT event UNKNOWN");
      break;
  }

  return ESP_OK;
}

void onResetButtonISR(void) {

  // Note: reset button current is pulled down via 10K OHM resistance

  unsigned long currentMillis = millis();

  if (digitalRead(resetButtonPin)) {

    previousResetButtonInterval = currentMillis;

  } else {

    if (currentMillis - previousResetButtonInterval >= resetButtonInterval) {

      Serial.println("Reset button pressed...");
      Serial.println("Erasing AP settings and rebooting...");
      WiFi.eraseAP();
      ESP.restart();
      //wiFiManager.disconnect(true, false);
      //WiFi.eraseAP();
      // esp_wifi_start();
      // Note: first restart after serial flashing causes puts the board in boot mode:(1,7) (purple led)
      // https://github.com/esp8266/Arduino/issues/1722
      // https://github.com/esp8266/Arduino/issues/1017
      // https://github.com/esp8266/Arduino/issues/1722#issuecomment-321818357


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

