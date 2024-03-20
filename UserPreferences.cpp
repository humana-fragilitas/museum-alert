#include <utility>
#include <Arduino.h>
#include <Preferences.h>
#include <ArduinoJson.h>

#include "UserPreferences.h"

std::pair<UserSettings, bool> UserPreferences::getPreferences() {

  bool hasPreferences;

  preferences.begin("preferences", false);

  String ssid = preferences.getString("ssid");
  String pass = preferences.getString("pass");

  preferences.end();

  if (!ssid || !pass) {

    Serial.println("\nNo user preferences found on this device");

    hasPreferences = false;

    return std::make_pair(settings, false);
    
  } else {

    hasPreferences = true;

    Serial.println("\nUser preferences found on this device");

    settings.ssid = ssid;
    settings.pass = ssid;

  }

  return std::make_pair(settings, true);

}

void UserPreferences::setPreferences(String conf) {

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
  
}






