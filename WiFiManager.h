#include <Arduino.h>
#include <vector>
#include <ArduinoJson.h>
#include <WiFi.h>

struct WiFiNetwork {
  String ssid;
  int32_t rssi;
  wifi_auth_mode_t encryptionType;
};

class WiFiManager {

  private:
    void(*_onWiFiConnectionSuccess)(void);
    void(*_onWiFiConnectionError)(void);

  public:
    WiFiManager(void(*onWiFiConnectionSuccess)(void), void(*onWiFiConnectionError)(void));
    void listNetworks(JsonArray* doc);
    void connectToWiFi(String ssid, String pass);
    wl_status_t connectToWiFi(void);
    bool eraseConfiguration(void);
    void disconnect(void);

};