#include <Arduino.h>
#include <vector>
#include <ArduinoJson.h>

struct WiFiNetwork {
  String ssid;
  int32_t rssi;
  wifi_auth_mode_t encryptionType;
};

class WiFiManager {

  public:
    void listNetworks(JsonDocument* doc);
    void connectToWiFi(String ssid, String pass);

};