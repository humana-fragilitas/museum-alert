#include<Arduino.h>

#include "Pins.h"

void pinSetup() {

  pinMode(triggerPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  pinMode(wiFiPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(triggerPin, LOW);
  digitalWrite(alarmPin, LOW);
  digitalWrite(wiFiPin, LOW);
  digitalWrite(echoPin, LOW);

}

