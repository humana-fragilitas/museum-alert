#include<Arduino.h>

void initializeSerial() {

  Serial.begin (9600);
  while (!Serial) {
    ;
  }
  Serial.println("\nSerial port ready");

}