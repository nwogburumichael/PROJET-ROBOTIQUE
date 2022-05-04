#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>


SoftwareSerial BTSerial(10, 11);

const int button = 2;
const int button2 = 3;
int buttonstate = 1;
int buttonstate2 = 1;
const int button3 = 4;
int buttonstate3 = 1;
int state =0;


const int vx = A0;
const int vy = A1;
const int boutonJ = 8;


void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);

  pinMode(boutonJ,INPUT_PULLUP);

}
void loop() 
{
 
 
 // Reading the button
/* buttonstate = digitalRead(button);
 if (buttonstate == HIGH) 
 {
   BTSerial.write('1'); // Sends '1' to the master to turn on LED
   state = '1';
 }


 buttonstate2 = digitalRead(button2);
 if (buttonstate2 == HIGH) 
 {
   BTSerial.write('2'); // Sends '1' to the master to turn on LED
 }

 buttonstate3 = digitalRead(button3);
 if (buttonstate3 == HIGH) 
 {
   BTSerial.write('3'); // Sends '1' to the master to turn on LED
 }
 if(buttonstate3 == LOW )
 {
   BTSerial.write('5');
 }
 if(buttonstate2 == LOW )
 {
   BTSerial.write('4');
 }
 if(buttonstate == LOW)
 {
   BTSerial.write('0');
   state='0';
 }
*/

float valeur_x = analogRead(vx);
float valeur_y = analogRead(vy);
int valeur_bouton = !digitalRead(boutonJ);
float a = 0.004882813;
 



   // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

}

