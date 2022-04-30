#include <Arduino.h>
#include <Wire.h>
#include <SerialTransfer.h>
int state = 0;
const int vx = A1;
const int vy = A0;
int valeur_x;
int valeur_y;
int memoire_x = 0;
int memoire_y = 0;
int diff_x;
int diff_y;
char x[4] ;
char y[4] ;
char buffer[9];
void setup()
{
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  Serial.begin(9600);
  Serial3.begin(9600); 
  Serial3.flush();
  Serial.flush();
}

void loop()
 {
  valeur_x = analogRead(vx);
  valeur_y = analogRead(vy);
  diff_x = valeur_x - memoire_x;
  diff_y = valeur_y - memoire_y;


 if (diff_x > 100 || diff_y > 100)
 {

  memoire_x = valeur_x;
  memoire_y = valeur_y;
  sprintf(buffer,"#%d;%d$",valeur_x,valeur_y);
  Serial.write(buffer);
  Serial3.write(buffer);
  delay(1000);

 }


 if(Serial3.available() )
 { 
    // Checks whether data is comming from the serial port
    state = Serial3.read(); // Reads the data from the serial port
    Serial.print(state);
    //Serial.println("MASTER");
 }
}