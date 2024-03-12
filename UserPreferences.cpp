#include "UserPreferences.h"

class UserPreferences {

  struct UserSettings {
    String ssid;
    String pass;
  };

  public:
    UserSettings getPreferences() {

      Preferences preferences;
      UserSettings settings;

      preferences.begin("preferences", false);

      String ssid = preferences.getString("ssid");
      String pass = preferences.getString("pass");

      preferences.end();

      if (!ssid || !pass) {

        Serial.println("\nNo user preferences found on this device");

        return NULL;
        
      } else {

        Serial.println("\nUser preferences found on this device");

        settings.ssid = ssid;
        settings.pass = ssid;

        return settings;

      }

    }

    void setPreferences(String conf) {

      /**
       * sample json  "{\"ssid\":\"Wind3 HUB - 0290C0\",\"pass\":\"73fdxdcc5x473dyz\"}"
       */

      StaticJsonDocument<192> doc;
      DeserializationError err = deserializeJson(doc, conf);

      if (err) {
        Serial.print(F("\nDeserializeJson() failed with code "));
        Serial.println(err.f_str());
      }

      Serial.printf("\nConfiguration: %s", conf);

      Serial.printf("\nSerialized 'ssid' property: %s", doc["ssid"].as<String>());
      Serial.printf("\nSerialized 'pass' property: %s", doc["pass"].as<String>());

      //Serial.printf("\nConfiguration: %c", conf);
      
    }

}






