//#include "mbed.h"

#include "Pins.h"
#include "PinSetup.h"
#include "SerialCom.h"
#include "UserPreferences.h"
#include "Sensor.h"
#include "WiFiManager.h"
#include "BLEManager.h"
#include "MQTTClient.h"

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_task_wdt.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_event.h>
#include <mqtt_client.h>
#include <esp_tls.h>

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
MQTTClient mqttClient;
BLEManager bleManager(&onWiFiCredentials, &onTLSCertificate);
WiFiManager wiFiManager(&onWiFiEvent);
Sensor sensor;

unsigned const int configureWiFiInterval = 4000;
unsigned int previousWiFiInterval = 0;

unsigned const int resetButtonInterval = 4000;
unsigned int previousResetButtonInterval = 0;

esp_err_t err;
int brokerSetUp = 0;

const unsigned char DSTroot_CA[] = R"CERT(-----BEGIN CERTIFICATE-----
MIIDSzCCAjOgAwIBAgIBATANBgkqhkiG9w0BAQUFADBpMRQwEgYDVQQDEwtleGFt
cGxlLm9yZzELMAkGA1UEBhMCVVMxETAPBgNVBAgTCFZpcmdpbmlhMRMwEQYDVQQH
EwpCbGFja3NidXJnMQ0wCwYDVQQKEwRUZXN0MQ0wCwYDVQQLEwRUZXN0MB4XDTI0
MDUwNTE2Mjc1MFoXDTI1MDUwNTE2Mjc1MFowaTEUMBIGA1UEAxMLZXhhbXBsZS5v
cmcxCzAJBgNVBAYTAlVTMREwDwYDVQQIEwhWaXJnaW5pYTETMBEGA1UEBxMKQmxh
Y2tzYnVyZzENMAsGA1UEChMEVGVzdDENMAsGA1UECxMEVGVzdDCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBAN3Rjx5EMPKbJ3fadAiTTsu4c7m0yTQK9Qqi
sNBfmnJVgV+eIaMlFehIYb25E5ltoYoJDRqFFfHYNukY2Vp0uAd6qph+yKthpEfw
lpvPegIc8G/kWnO+qo/2k4NOvyr2/piN1f8XfRhG+DsMl2hc9l9NVGLakVxliYtV
czWltYchpHdFlD6tiNbuAalSEDyOVo5lDLSPd2Kbz6GA+j5MvFNd7CQQYGyXnL8G
lhBg/2Qs5B76mWPDSQ4TypOi7mpUZhByWFX69Eil46olPV9jaIwUTposB0GQivbD
lgzHYUOEOpA/qVLvwQaO2bu1jHUdgztNgK+lx4Cq/kqhAV5ustUCAwEAATANBgkq
hkiG9w0BAQUFAAOCAQEAPCvvAwLR1ZF7Iz3XdtC/SyMIexUqTkeHHan+EzgbIW8m
Q5PgjLalpiJChxFBINO4t4iPTPxKi5q0v4a69NoHK03YDWAKZ3XRlIhZ51ZuBdZf
0Lh3YL/luvC1Z3eexBs15m4fcjnOm2xJ9ZG+ej8k0KVSZSA5GVUHdqT0S+JIpiLQ
7MGQztOKzzCOk8siXyvT80BoAlQgjtczNMINCvW01Mq3kS/HXZIoS6Thhpz2CiKu
CJaY5qt9ftK7UPfdHyAzUdB4E17A2CmSwt5Hk7DkxHiubrOL6rVBBZNqlQsXGhbn
sJPRQ0RKsydyb+0HJEK4A7bSqEzMtL8lQH6obIoo6A==
-----END CERTIFICATE-----
)CERT";

const unsigned char privateKey[] = R"KEY(-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA3dGPHkQw8psnd9p0CJNOy7hzubTJNAr1CqKw0F+aclWBX54h
oyUV6EhhvbkTmW2higkNGoUV8dg26RjZWnS4B3qqmH7Iq2GkR/CWm896Ahzwb+Ra
c76qj/aTg06/Kvb+mI3V/xd9GEb4OwyXaFz2X01UYtqRXGWJi1VzNaW1hyGkd0WU
Pq2I1u4BqVIQPI5WjmUMtI93YpvPoYD6Pky8U13sJBBgbJecvwaWEGD/ZCzkHvqZ
Y8NJDhPKk6LualRmEHJYVfr0SKXjqiU9X2NojBROmiwHQZCK9sOWDMdhQ4Q6kD+p
Uu/BBo7Zu7WMdR2DO02Ar6XHgKr+SqEBXm6y1QIDAQABAoIBAHsEj3TfqK3Dsn3b
32IqIBcHctbZFoUQVpnRZHILs2IZXaij0E/kb2PlUJ+hlucOT/p3zpaYnHUFzl4z
88cg2Db9psLv/WZevndPTJeY1zd4yTek0y1B3uH8hA7ci1TOqp/8eWQBqNTf9yb4
crfkZpawEk7InLk7bq6hc0WbYzNPQs+WdRyqMDy6rQU7tz5/9wXm0442V1l0qUAH
U5qoNKOGIxOt9ODmbKRvZfu/Ti/LzoX+R6bXUvzNmASgRJvMoZmXI71ltp+dp4QO
bBz74JV7VQglW+VuTM5ZgsjLlxAidAzR7PiDuQRUKwi6xrPBZFe81Kt5z+UEoqyu
steyj7kCgYEA7udOB80Y6v9PrrirFeE/nOjipkA18tE3vixXyfc2mQAUqrezOtsJ
0LYk1OBDyV+JXLQ5/n0QWe2JkpijpVMP3uPgbXs5swc0Gd3IJtfY6rppKMmZ9G2v
4QC4bUwz8hzGpiGXLTuInE9JnYD1Fqau3vD5QmAfsRVmegg5BGI0VKMCgYEA7bFC
YxOrJi6nkRPkb3kbU2CG99n5CkuwbY1xSZ641O8b9lBIEaTNKa2K5GzVVygcBoYe
D0O8US9FSTigBYtHMdWii2mVzABnU++RKnPRybkvrEqS6yAZnYSSabfzvzHUjdvc
BA3ugdC+xFDpZohSRSsIsV6mTv/+IHy3TOlB2icCgYBBQjHQc7fwyVkM0yj4yxTD
WeI/o56Y+4mMizRX2Q2y8ZCzqYZt91NRDdA1ziT2JwCwsBJ/b849omNIBiX9jRjy
u7Ccd2Klgrw6rJh189QvGkiLebZyTFwUzEuUn5I4+p9Q1jAjjPWeLRJg0c8hJtrx
z0VnWjaN+tiJUSGyDtlYnQKBgQCbEquyQhHGLmgKgcTmaUfHpNFJgL821Vy8jKwd
kN0bpwhCMexi/ncPuvZDwzGI1FU8eGHCKboB6Wo1tCjKlSyUucF7XR5q4tSG8WRv
IvL6vcP5jwm9Nssfdm+jY202DweSqZ8oUgKKVSswn203BLdQAxx/w7WTEva0MUnR
dtkMvwKBgQC1SGDAQmNfk5krtsULtCNM+vw3FEuQgl74t9SETuHvhLZLS/H3MTT+
avqG/cjz3bBQZU98VMzTlLkzKm4gWegfFm5OMCkuhniW+7AgS3GTlIAfBSt4aVM7
34xcrXLK156OJAIsqXRCXVzCFJ4RPjJyfM80fKaa2nOb5kSEYklKeQ==
-----END RSA PRIVATE KEY-----
)KEY";

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

  appState = (wiFiManager.connectToWiFi() != WL_CONNECTED) ?
    CONFIGURE_WIFI : SET_SSL_CERTIFICATE;

WiFiClientSecure espClient;
globalClient = new PubSubClient(espClient);
espClient.setCertificate((char*)DSTroot_CA); // for client verification
espClient.setPrivateKey((char*)privateKey);  // for client verification
char* mqtt_server = "museum-alert-iot-hub.azure-devices.net";
globalClient->setServer(mqtt_server, 8883);
globalClient->setCallback(callback);
if (globalClient->connect("MAS-EC357A188534", "museum-alert-iot-hub.azure-devices.net/MAS-EC357A188534/?api-version=2021-04-12", NULL)) {
  Serial.println("MQTT Server connected!");
}

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
        mqttClient.connect();
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

// this is event handler that we will register in mqtt_setup(). You will need at least two events: MQTT_EVENT_CONNECTED and MQTT_EVENT_DATA. more possible events see ESP-IDF example
static void mqtt_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

  Serial.printf("\nEvent base: %s; id: %d; name; %s; data: %s", event_base, event_id, esp_err_to_name(event_id), event_data);

}

int setupMqttBroker(void) {

  return 1;

}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
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

