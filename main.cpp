#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>
#include <SerialTransfer.h> 
#include <stdio.h>
String Oled ="";
int Valeur_x;
int Valeur_y;
int bouton;
#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
 
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);
 
byte tailleDeCaractere=1;

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation
    ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(tailleDeCaractere);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
    ecranOLED.setCursor(0, 0);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
   ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond 
    
}
void loop() {
  delay(10);
  if (Serial3.available()) {
  Oled = Serial3.readStringUntil('/');
  Valeur_x = Oled.toInt();
  Oled = Serial3.readStringUntil('/');
  Valeur_y = Oled.toInt();
  Oled = Serial3.readStringUntil(';');
  bouton = Oled.toInt();




  ecranOLED.setCursor(0, 0);                                 // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
  ecranOLED.print("X : ");
  ecranOLED.print(String(Valeur_x));
 
  ecranOLED.setCursor(0,20);
  ecranOLED.print("Y : ");
  ecranOLED.print(String(Valeur_y));
 
  ecranOLED.setCursor(0,40);
ecranOLED.print("Bouton = ");
if (bouton ==1){
  ecranOLED.print("ON");
}
if (bouton ==0){
  ecranOLED.print("OFF");
}
 
    ecranOLED.display();                            // Transfert le buffer à l'écran
    delay(10);
    ecranOLED.clearDisplay();

  Serial.print("Angle X : ");
  Serial.println(Valeur_x);
  Serial.print("Angle Y : ");
  Serial.println(Valeur_y);
  Serial.print("Bouton : ");
  Serial.println(bouton);
  }
} 