#include <ArduinoBLE.h>

#ifndef BLE_MANAGER
#define BLE_MANAGER

class BLEManager {

  private:
    static const char* deviceServiceUuid;
    static const char* deviceServiceConfigurationCharacteristicUuid;
    static const char* deviceServiceSsidsCharacteristicUuid;
    static BLEService configurationService; 
    static BLEStringCharacteristic configurationCharacteristic;
    static BLEStringCharacteristic wiFiSsidsCharacteristic;

  public:
    void initializeBLEConfigurationService();
    void broadCastWiFiSsids(char json[]);
    void configureViaBLE();

};

#endif