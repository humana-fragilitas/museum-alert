#include <WiFi.h>

class WiFiManager {

  int status = WL_IDLE_STATUS;
  char ssid[] = "Wind3 HUB - 0290C0";
  char pass[] = "73fdxdcc5x473dyz";

  public:
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

}