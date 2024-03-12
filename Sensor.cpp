#include "Pins.h"

class Sensor {

  private:
    float durationMicroSec, distanceincm;

  public:

    void detect() {

      /* Send 10 microsec pulse to TRIG pin*/
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);
      /* measure pulse duration from ECHO pin*/
      durationMicroSec = pulseIn(echoPin, HIGH);
      /* calculate distance*/
      distanceincm = 0.017 * durationMicroSec;
      /*Display distance on Serial Monitor*/
      //Serial.print("distance: ");
      //Serial.print(distanceincm);  /*Print distance in cm*/
      //Serial.println(" cm");

      if (distanceincm < 10.0) {
        digitalWrite(alarmPin, HIGH);
        Serial.printf("\nAlarm! Distance detected: %.2f cm", distanceincm);
      } else {
        digitalWrite(alarmPin, LOW);
      }

      delay(1000);
          
    }

}