#include <WiFi.h>
#include <ArduinoBLE.h>
#include <ArduinoJson.h>

int triggerPin = 9; /*TRIG pin D9 of Arduino NANO*/
int echoPin = 8;    /*ECHO pin D8 of Arduino NANO*/
int wiFiPin = 3;  
int alarmPin = 2;   

float durationMicroSec, distanceincm;
char ssid[] = "Wind3 HUB - 0290C0";
char pass[] = "";
int status = WL_IDLE_STATUS;
String configuration = "";

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";
BLEService configurationService(deviceServiceUuid); 
BLEStringCharacteristic configurationCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite, 512);

void setup() {

  initializeSerial();
  setUpPins();
  initializeBLE();
  listNetworks();
  connectToWiFi();

}

void loop() {

  configureViaBLE();
  initializeSensor();

}

void initializeSerial() {

  Serial.begin (9600);
  while (!Serial) {
    ;
  }
  Serial.println("\nSerial port ready");

}

void setUpPins() {

  pinMode(triggerPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(wiFiPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

void listNetworks() {

  byte numSsid;

  Serial.println("\nScanning WiFi networks");

    while(!(numSsid = WiFi.scanNetworks())){
        Serial.print(".");
        delay(100);
    }

  // print the list of networks seen:

  Serial.printf("\nNumber of available networks: %d", numSsid);

  // print the network number and name for each network found:

  for (int thisNet = 0; thisNet<numSsid; thisNet++) {

    Serial.printf("\n%d) %s | signal: %d dbm | encryption: %d",
      thisNet, WiFi.SSID(thisNet), WiFi.RSSI(thisNet), WiFi.encryptionType(thisNet));

    Serial.println(WiFi.SSID(thisNet));

  }

}

void connectToWiFi() {

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    digitalWrite(wiFiPin, LOW);
    delay(100);
   
    WiFi.begin(ssid, pass);
    Serial.println("\nConnecting to WiFi");

    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
    }

    digitalWrite(wiFiPin, HIGH);
    Serial.printf("\nConnected to WiFi network: %s", ssid);
    Serial.printf("\nLocal ESP32 IP: %s", WiFi.localIP().toString());

}

void initializeBLE() {

  char sensorName[33];
  uint64_t chipid = ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid >> 32);

  if (!BLE.begin()) {
    Serial.println("\nStarting Bluetooth® Low Energy module failed!");
    while (1);
  }

  Serial.println("\nBluetooth® Low Energy module initialized");

  snprintf(sensorName, 33, "MAS-%04X%08X", chip, (uint32_t)chipid);
  BLE.setLocalName(sensorName); 
  BLE.setAdvertisedService(configurationService);
  configurationService.addCharacteristic(configurationCharacteristic);
  BLE.addService(configurationService);
  configurationCharacteristic.writeValue("");
  BLE.advertise();

  Serial.printf("Sensor Bluetooth® Low Energy module advertising itself with local name %s", sensorName);

}

void configureViaBLE() {

  BLEDevice central = BLE.central();
  Serial.println("\nDiscovering central device...");
  delay(500);

  if (central) {
    Serial.println("\nConnected to central device!");
    Serial.println("\nDevice MAC address: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (configurationCharacteristic.written()) {
        configurationCharacteristic.value();
        writeConfiguration(configurationCharacteristic.value());
       }
    }
    
    Serial.println("\nDisconnected to central device!");
  }

}


void writeConfiguration(String conf) {

  /**
   * sample json  "{\"ssid\":\"Wind3 HUB - 0290C0\",\"pass\":\"73fdxdcc5x473dyz\"}"
   */

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, conf);

  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.f_str());
  }

  Serial.printf("\nConfiguration: %s", conf);

  Serial.printf("\nSerialized 'ssid' property: %c", doc["ssid"]);
  Serial.printf("\nSerialized 'ssid' property: %c", doc["pass"]);

  //Serial.printf("\nConfiguration: %c", conf);
  
}

void initializeSensor() {

  /* Send 10 microsec pulse to TRIG pin*/
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  /* measure pulse duration from ECHO pin*/
  durationMicroSec = pulseIn(echoPin, HIGH);
  /* calculate distance*/
  distanceincm = 0.017 * durationMicroSec;
  /*Display distance on Serial Monitor*/
  //Serial.print("distance: ");
  //Serial.print(distanceincm);  /*Print distance in cm*/
  //Serial.println(" cm");

  if (distanceincm < 10.0) {
    digitalWrite(alarmPin, HIGH);
    Serial.printf("\nAlarm! Distance detected: %.2f cm", distanceincm);
  } else {
    digitalWrite(alarmPin, LOW);
  }

  delay(1000);

}
