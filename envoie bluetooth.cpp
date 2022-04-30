#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
 
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);
 
SoftwareSerial BTSerial(10, 11);
byte tailleDeCaractere=1;

const int Vry = A1, Vrx = A0, SW =  2;
int valbutt;
char tram[13];
void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH); 
 
  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation
    ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(tailleDeCaractere);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
    ecranOLED.setCursor(0, 0);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
   ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond 
    pinMode(SW,INPUT_PULLUP);
}
void loop() {
  int valeur_x = analogRead(Vrx);
  int valeur_y = analogRead(Vry);
  int bouton = !digitalRead(SW);
 
  ecranOLED.setCursor(0, 0);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
  ecranOLED.print("X : ");
  ecranOLED.print(String(valeur_x));
 
  ecranOLED.setCursor(0,10);
  ecranOLED.print("Y : ");
  ecranOLED.print(String(valeur_y));
 
  ecranOLED.setCursor(0,20);
ecranOLED.print("Bouton = ");
if (bouton ==1){
  ecranOLED.print("ON");
  valbutt= 0;
  
}
if (bouton ==0){
  ecranOLED.print("OFF");
  valbutt=1;
  
}
ecranOLED.setCursor(0,30);
sprintf(tram , "%d/%d/%d;" , valeur_x , valeur_y,valbutt);
ecranOLED.print(tram);
BTSerial.write(tram);

    ecranOLED.display();                            // Transfert le buffer à l'écran
    delay(200);
    ecranOLED.clearDisplay();
    if (Serial.available()){
Serial.write(BTSerial.read());
    }
} 
