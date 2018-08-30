#include <Wire.h>

#include "DS3231.h";



DS3231 ds;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
      Serial.print("Hi");
}

void loop() {
  // put your main code here, to run repeatedly:


      Serial.print(ds.getSecond());
      Serial.print("\n");

delay(1000);
}
