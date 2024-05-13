#include <Arduino.h>

// C99 libraries
#include <cstdlib>
#include <string.h>
#include <time.h>

// Libraries for MQTT client and WiFi connection
#include <WiFi.h>
#include <mqtt_client.h>

// Azure IoT SDK for C includes
#include <az_core.h>
#include <az_iot.h>
#include <azure_ca.h>

class MQTTClient {

  private:
     const unsigned char IOT_CONFIG_DEVICE_CERT[];
     const unsigned char IOT_CONFIG_DEVICE_CERT_PRIVATE_KEY[];
     const char* host;
     const char* mqtt_broker_uri;
     const char* device_id;
     const int mqtt_port;
     esp_mqtt_client_handle_t mqtt_client;
     az_iot_hub_client client;
     char mqtt_client_id[128];
     char mqtt_username[128];
     char mqtt_password[200];
     uint8_t sas_signature_buffer[256];
     unsigned long next_telemetry_send_time_ms;
     char telemetry_topic[128];
     uint32_t telemetry_send_count;
     String telemetry_payload;
     char incoming_data[INCOMING_DATA_BUFFER_SIZE];

    static void initializeIoTHubClient();
    static int initializeMqttClient();

  public:
    void connect();

};