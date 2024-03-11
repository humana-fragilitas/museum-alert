#include <ArduinoBLE.h>

class BLEManager {

  const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
  const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";
  BLEService configurationService(deviceServiceUuid); 
  BLEStringCharacteristic configurationCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite, 512);

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
          // setPreferences(configurationCharacteristic.value());
        }
      }
      
      Serial.println("\nDisconnected to central device!");
    }

  }

}