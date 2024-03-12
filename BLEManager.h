#ifndef BLE_MANAGER
#define BLE_MANAGER

class BLEManager {

  private:
    const char* deviceServiceUuid;
    const char* deviceServiceCharacteristicUuid;
    BLEService configurationService; 
    BLEStringCharacteristic configurationCharacteristic;

  public:
    void initializeBLE();
    void configureViaBLE();

}

#endif