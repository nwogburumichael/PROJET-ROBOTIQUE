#include <Arduino.h>
#include <MFRC522.h>
#include<Wire.h>
#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;

// Set up a new SoftwareSerial object
SoftwareSerial mySerial(rxPin, txPin);

#define Led 13
#define RST_PIN 1    // Configurable, see typical pin layout above
#define SS_PIN 4    // Configurable, see typical pin layout above


MFRC522 mfrc522(SS_PIN,RST_PIN);  // Create MFRC522 instance
byte nuidPICC[4];
byte MichaelBadge[4] = {0x9C,0xF5,0x37,0x63}; // Code du porte clé
byte MichaelCarte[4] = {0x93,0x78,0xA2,0x4}; // Code de la carte

void setup() {
pinMode(Led,OUTPUT);
mySerial.begin(9600);
SPI.begin();
mfrc522.PCD_Init();   
digitalWrite(Led,HIGH);
delay(2000);
digitalWrite(Led,LOW);
   
} 


void loop() 
{
  // Initialisé la boucle si aucun badge n'est présent 
  if ( !mfrc522.PICC_IsNewCardPresent())
    return;

  // Vérifier la présence d'un nouveau badge 
  if ( !mfrc522.PICC_ReadCardSerial())
    return;

  // Enregistrer l'ID du badge (4 octets) 
  for (byte i = 0; i < 4; i++) 
  {
    nuidPICC[i] = mfrc522.uid.uidByte[i];
  }
   // Affichage de l'ID 
 
 mySerial.print("Un badge est détecté");
 mySerial.print(" L'UID du tag est:");
 digitalWrite(Led,HIGH);
 


 
 
  for (byte i = 0; i < 4; i++) 
  {
   mySerial.print(nuidPICC[i], HEX);
   mySerial.print(" ");
  }

  delay(2000);


   for (byte i = 0; i < 4; i++) 
   {
     if (!nuidPICC[i] == MichaelBadge
    [i] )
     {
       break;
     }
     if (i == 3 && nuidPICC[i] == MichaelBadge
    [i])
     {

       mySerial.print("MichaelBadge");
     }

   }

   for (byte i = 0; i < 4; i++) 
   {
     if (!nuidPICC[i] == MichaelCarte[i] )
     {
       break;
     }
     if (i == 3 && nuidPICC[i] == MichaelCarte[i])
     {

       mySerial.print("MichaelCarte");

     }

   }
 

}
  