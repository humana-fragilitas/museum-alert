#include <ArduinoBLE.h>

#ifndef BLE_MANAGER
#define BLE_MANAGER

class BLEManager {

  private:
    static const char* deviceServiceUuid;
    static const char* deviceServiceCharacteristicUuid;
    static BLEService configurationService; 
    static BLEStringCharacteristic configurationCharacteristic;

  public:
    void initializeBLE();
    void configureViaBLE();

};

#endif