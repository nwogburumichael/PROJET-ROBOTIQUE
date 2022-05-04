#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

const int ledPin = 13;
char state ;
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {

  state = char (Serial1.read());



  if (Serial1.available())
    Serial.write(Serial1.read());
}
}