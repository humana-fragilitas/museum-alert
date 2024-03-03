#include <WiFi.h>
#include <ArduinoBLE.h>

int triggerPin = 9; /*TRIG pin D9 of Arduino NANO*/
int echoPin = 8;    /*ECHO pin D8 of Arduino NANO*/
int wiFiPin = 3;  
int alarmPin = 2;   
float durationMicroSec, distanceincm;
char ssid[] = "Wind3 HUB - 0290C0";
char pass[] = "73fdxdcc5x473dyz";
int status = WL_IDLE_STATUS;

void setup() {

  Serial.begin (9600);  /*Baud rate for serial communication*/
  /* Trigger pin defined as output*/
  pinMode(triggerPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(wiFiPin, OUTPUT);
  /* Echo pin defined as input*/
  pinMode(echoPin, INPUT);

  initializeBle();
  listNetworks();
  connectToWiFi();

}

void loop() {
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
    Serial.print("distance: ");
    Serial.print(distanceincm);  /*Print distance in cm*/
    Serial.print(" cm");  /*Print distance in cm*/
    Serial.println("\nAlarm!");
  } else {
    digitalWrite(alarmPin, LOW);
  }

  delay(1000);

}

void listNetworks() {

  // scan for nearby networks:

  Serial.println("** Scan Networks **");

  byte numSsid = WiFi.scanNetworks();

  // print the list of networks seen:

  Serial.print("number of available networks:");

  Serial.println(numSsid);

  // print the network number and name for each network found:

  for (int thisNet = 0; thisNet<numSsid; thisNet++) {

    Serial.print(thisNet);

    Serial.print(") ");

    Serial.print(WiFi.SSID(thisNet));

    Serial.print("\tSignal: ");

    Serial.print(WiFi.RSSI(thisNet));

    Serial.print(" dBm");

    Serial.print("\tEncryption: ");

    Serial.println(WiFi.encryptionType(thisNet));

  }
}

void connectToWiFi() {

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    digitalWrite(wiFiPin, LOW);
    delay(100);
   
    WiFi.begin(ssid, pass);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    digitalWrite(wiFiPin, HIGH);
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

}

void initializeBle() {

  Serial.println("\nInitializing BLE");

  while(!BLE.begin()) {
    Serial.print(".");
        delay(100);
  }

}
