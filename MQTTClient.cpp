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

#define IOT_CONFIG_IOTHUB_FQDN "museum-alert-iot-hub.azure-devices.net"
#define IOT_CONFIG_DEVICE_ID "MAS-EC357A188534"
#define AZ_IOT_DEFAULT_MQTT_CONNECT_PORT 8883
#define INCOMING_DATA_BUFFER_SIZE 128
#define AZURE_SDK_CLIENT_USER_AGENT "c%2F" AZ_SDK_VERSION_STRING "(ard;esp32)"

class MQTTClient {

  private:
 const unsigned char IOT_CONFIG_DEVICE_CERT[] = R"CERT(-----BEGIN CERTIFICATE-----
MIIDSzCCAjOgAwIBAgIBATANBgkqhkiG9w0BAQUFADBpMRQwEgYDVQQDEwtleGFt
cGxlLm9yZzELMAkGA1UEBhMCVVMxETAPBgNVBAgTCFZpcmdpbmlhMRMwEQYDVQQH
EwpCbGFja3NidXJnMQ0wCwYDVQQKEwRUZXN0MQ0wCwYDVQQLEwRUZXN0MB4XDTI0
MDUwNTE2Mjc1MFoXDTI1MDUwNTE2Mjc1MFowaTEUMBIGA1UEAxMLZXhhbXBsZS5v
cmcxCzAJBgNVBAYTAlVTMREwDwYDVQQIEwhWaXJnaW5pYTETMBEGA1UEBxMKQmxh
Y2tzYnVyZzENMAsGA1UEChMEVGVzdDENMAsGA1UECxMEVGVzdDCCASIwDQYJKoZI
hvcNAQEBBQADggEPADCCAQoCggEBAN3Rjx5EMPKbJ3fadAiTTsu4c7m0yTQK9Qqi
sNBfmnJVgV+eIaMlFehIYb25E5ltoYoJDRqFFfHYNukY2Vp0uAd6qph+yKthpEfw
lpvPegIc8G/kWnO+qo/2k4NOvyr2/piN1f8XfRhG+DsMl2hc9l9NVGLakVxliYtV
czWltYchpHdFlD6tiNbuAalSEDyOVo5lDLSPd2Kbz6GA+j5MvFNd7CQQYGyXnL8G
lhBg/2Qs5B76mWPDSQ4TypOi7mpUZhByWFX69Eil46olPV9jaIwUTposB0GQivbD
lgzHYUOEOpA/qVLvwQaO2bu1jHUdgztNgK+lx4Cq/kqhAV5ustUCAwEAATANBgkq
hkiG9w0BAQUFAAOCAQEAPCvvAwLR1ZF7Iz3XdtC/SyMIexUqTkeHHan+EzgbIW8m
Q5PgjLalpiJChxFBINO4t4iPTPxKi5q0v4a69NoHK03YDWAKZ3XRlIhZ51ZuBdZf
0Lh3YL/luvC1Z3eexBs15m4fcjnOm2xJ9ZG+ej8k0KVSZSA5GVUHdqT0S+JIpiLQ
7MGQztOKzzCOk8siXyvT80BoAlQgjtczNMINCvW01Mq3kS/HXZIoS6Thhpz2CiKu
CJaY5qt9ftK7UPfdHyAzUdB4E17A2CmSwt5Hk7DkxHiubrOL6rVBBZNqlQsXGhbn
sJPRQ0RKsydyb+0HJEK4A7bSqEzMtL8lQH6obIoo6A==
-----END CERTIFICATE-----
)CERT";

 const unsigned char IOT_CONFIG_DEVICE_CERT_PRIVATE_KEY[] = R"KEY(-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA3dGPHkQw8psnd9p0CJNOy7hzubTJNAr1CqKw0F+aclWBX54h
oyUV6EhhvbkTmW2higkNGoUV8dg26RjZWnS4B3qqmH7Iq2GkR/CWm896Ahzwb+Ra
c76qj/aTg06/Kvb+mI3V/xd9GEb4OwyXaFz2X01UYtqRXGWJi1VzNaW1hyGkd0WU
Pq2I1u4BqVIQPI5WjmUMtI93YpvPoYD6Pky8U13sJBBgbJecvwaWEGD/ZCzkHvqZ
Y8NJDhPKk6LualRmEHJYVfr0SKXjqiU9X2NojBROmiwHQZCK9sOWDMdhQ4Q6kD+p
Uu/BBo7Zu7WMdR2DO02Ar6XHgKr+SqEBXm6y1QIDAQABAoIBAHsEj3TfqK3Dsn3b
32IqIBcHctbZFoUQVpnRZHILs2IZXaij0E/kb2PlUJ+hlucOT/p3zpaYnHUFzl4z
88cg2Db9psLv/WZevndPTJeY1zd4yTek0y1B3uH8hA7ci1TOqp/8eWQBqNTf9yb4
crfkZpawEk7InLk7bq6hc0WbYzNPQs+WdRyqMDy6rQU7tz5/9wXm0442V1l0qUAH
U5qoNKOGIxOt9ODmbKRvZfu/Ti/LzoX+R6bXUvzNmASgRJvMoZmXI71ltp+dp4QO
bBz74JV7VQglW+VuTM5ZgsjLlxAidAzR7PiDuQRUKwi6xrPBZFe81Kt5z+UEoqyu
steyj7kCgYEA7udOB80Y6v9PrrirFeE/nOjipkA18tE3vixXyfc2mQAUqrezOtsJ
0LYk1OBDyV+JXLQ5/n0QWe2JkpijpVMP3uPgbXs5swc0Gd3IJtfY6rppKMmZ9G2v
4QC4bUwz8hzGpiGXLTuInE9JnYD1Fqau3vD5QmAfsRVmegg5BGI0VKMCgYEA7bFC
YxOrJi6nkRPkb3kbU2CG99n5CkuwbY1xSZ641O8b9lBIEaTNKa2K5GzVVygcBoYe
D0O8US9FSTigBYtHMdWii2mVzABnU++RKnPRybkvrEqS6yAZnYSSabfzvzHUjdvc
BA3ugdC+xFDpZohSRSsIsV6mTv/+IHy3TOlB2icCgYBBQjHQc7fwyVkM0yj4yxTD
WeI/o56Y+4mMizRX2Q2y8ZCzqYZt91NRDdA1ziT2JwCwsBJ/b849omNIBiX9jRjy
u7Ccd2Klgrw6rJh189QvGkiLebZyTFwUzEuUn5I4+p9Q1jAjjPWeLRJg0c8hJtrx
z0VnWjaN+tiJUSGyDtlYnQKBgQCbEquyQhHGLmgKgcTmaUfHpNFJgL821Vy8jKwd
kN0bpwhCMexi/ncPuvZDwzGI1FU8eGHCKboB6Wo1tCjKlSyUucF7XR5q4tSG8WRv
IvL6vcP5jwm9Nssfdm+jY202DweSqZ8oUgKKVSswn203BLdQAxx/w7WTEva0MUnR
dtkMvwKBgQC1SGDAQmNfk5krtsULtCNM+vw3FEuQgl74t9SETuHvhLZLS/H3MTT+
avqG/cjz3bBQZU98VMzTlLkzKm4gWegfFm5OMCkuhniW+7AgS3GTlIAfBSt4aVM7
34xcrXLK156OJAIsqXRCXVzCFJ4RPjJyfM80fKaa2nOb5kSEYklKeQ==
-----END RSA PRIVATE KEY-----
)KEY";

  // Translate iot_configs.h defines into variables used by the sample
   const char* host = IOT_CONFIG_IOTHUB_FQDN;
   const char* mqtt_broker_uri = "mqtts://" IOT_CONFIG_IOTHUB_FQDN;
   const char* device_id = IOT_CONFIG_DEVICE_ID;
   const int mqtt_port = AZ_IOT_DEFAULT_MQTT_CONNECT_PORT;

  // Memory allocated for the sample's variables and structures.
   esp_mqtt_client_handle_t mqtt_client;
   az_iot_hub_client client;

   char mqtt_client_id[128];
   char mqtt_username[128];
   char mqtt_password[200];
   uint8_t sas_signature_buffer[256];
   unsigned long next_telemetry_send_time_ms = 0;
   char telemetry_topic[128];
   uint32_t telemetry_send_count = 0;
   String telemetry_payload = "{}";
   char incoming_data[INCOMING_DATA_BUFFER_SIZE];

  static void initializeIoTHubClient()
    {
      az_iot_hub_client_options options = az_iot_hub_client_options_default();
      options.user_agent = AZ_SPAN_FROM_STR(AZURE_SDK_CLIENT_USER_AGENT);

      if (az_result_failed(az_iot_hub_client_init(
              &client,
              az_span_create((uint8_t*)host, strlen(host)),
              az_span_create((uint8_t*)device_id, strlen(device_id)),
              &options)))
      {
        Logger.Error("Failed initializing Azure IoT Hub client");
        return;
      }

      size_t client_id_length;
      if (az_result_failed(az_iot_hub_client_get_client_id(
              &client, mqtt_client_id, sizeof(mqtt_client_id) - 1, &client_id_length)))
      {
        Logger.Error("Failed getting client id");
        return;
      }

      if (az_result_failed(az_iot_hub_client_get_user_name(
              &client, mqtt_username, sizeofarray(mqtt_username), NULL)))
      {
        Logger.Error("Failed to get MQTT clientId, return code");
        return;
      }

      Logger.Info("Client ID: " + String(mqtt_client_id));
      Logger.Info("Username: " + String(mqtt_username));
    }
  
  static int initializeMqttClient()
    {

      esp_mqtt_client_config_t mqtt_config;
      memset(&mqtt_config, 0, sizeof(mqtt_config));
      mqtt_config.uri = mqtt_broker_uri;
      mqtt_config.port = mqtt_port;
      mqtt_config.client_id = mqtt_client_id;
      mqtt_config.username = mqtt_username;

      Logger.Info("MQTT client using X509 Certificate authentication");
      mqtt_config.client_cert_pem = IOT_CONFIG_DEVICE_CERT;
      mqtt_config.client_key_pem = IOT_CONFIG_DEVICE_CERT_PRIVATE_KEY;

      mqtt_config.keepalive = 30;
      mqtt_config.disable_clean_session = 0;
      mqtt_config.disable_auto_reconnect = false;
      //mqtt_config.event_handle = mqtt_event_handler;
      mqtt_config.user_context = NULL;
      mqtt_config.cert_pem = (const char*)ca_pem;

      mqtt_client = esp_mqtt_client_init(&mqtt_config);

      if (mqtt_client == NULL)
      {
        Logger.Error("Failed creating mqtt client");
        return 1;
      }

      esp_err_t start_result = esp_mqtt_client_start(mqtt_client);

      if (start_result != ESP_OK)
      {
        Logger.Error("Could not start mqtt client; error code:" + start_result);
        return 1;
      }
      else
      {
        Logger.Info("MQTT client started");
        return 0;
      }
    }

  public:
    void connect() {

      MQTTClient::initializeIoTHubClient();
      MQTTClient::initializeMqttClient();

    }

};