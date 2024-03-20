#include <ArduinoBLE.h>

#include "UserPreferences.h"

#ifndef BLE_MANAGER
#define BLE_MANAGER

class BLEManager {

  private:
    static const char* deviceServiceUuid;
    static const char* deviceServiceConfigurationCharacteristicUuid;
    static const char* deviceServiceSsidsCharacteristicUuid;
    static BLEService configurationService;
    static BLEStringCharacteristic wiFiSsidsCharacteristic;
    static BLEStringCharacteristic configurationCharacteristic;
    std::function<std::pair<UserSettings, bool>(void)> onWiFiCredentials;
    std::function<char(void)> onTLSCertificate;

  public:
    BLEManager(void (*onWiFiCredentials)(std::pair<UserSettings, bool>), void (*onTLSCertificate)(char[]));
    void initializeBLEConfigurationService();
    void broadCastWiFiSsids(char json[]);
    void configureViaBLE();

};

#endif