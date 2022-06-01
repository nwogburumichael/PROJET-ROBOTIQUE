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



const int Vry = A1, Vrx = A0, SW =  2, SERVy = A3, SERVx = A2, BSER = 3, MANU_BOUT = 4;  //connection hardware avec le code
int valbutt, man_x, man_y, ValSX, ValSY,man, distance;
String tram_rec;   //info recu
char tram[40], JOY1[40],JOY2[40],DIST[40];     //tram envoi


void reception(){     //fonction pour la reception
tram_rec = BTSerial.readStringUntil("F");     //on va stocker le string lue avant le caractère F
distance = tram_rec.toInt();
sprintf(DIST,"distance : %d", distance);
}
void envoi(){ //fonction envoi au robot
sprintf(tram , "<%d/%d/%d/%d/%d/%d>" , man_x , man_y,valbutt,ValSX,ValSY,man);  //tram qu'on envoi
BTSerial.write(tram);  
}

void data_envoi(){  //fonction prise de donnée a envoyé
man_x = analogRead(Vrx);
man_y = analogRead(Vry);
ValSX = analogRead(SERVx);
ValSY = analogRead(SERVy);
valbutt = !digitalRead(SW);
man = digitalRead(MANU_BOUT);
sprintf(JOY1,"X : %d", man_x);
sprintf(JOY2,"X2 : %d", ValSX); 
}

void affichage_test(){       
ecranOLED.setCursor(0, 0);         //afficher X du joystick man             
ecranOLED.print(JOY1);

ecranOLED.setCursor(0,10);        //afficher X du joystick SERV
ecranOLED.print(JOY2);

ecranOLED.setCursor(0,20);  
ecranOLED.print("Mode = ");
if (man ==1){
  ecranOLED.print("MANU");            //afficher manu sur l'oled si l'interrupteur est ON
}
else{
  ecranOLED.print("AUTO");            //afficher auto sur l'oled si l'intterupteur est OFF
}

ecranOLED.setCursor(0,30);
ecranOLED.print(DIST);              //afficher la distance entre robot et obstacle
ecranOLED.display();
}

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(SW, INPUT);               //bouton premier joystick
  pinMode(MANU_BOUT, INPUT);       //switch manu auto
  digitalWrite(SW, HIGH); 

  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)){
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation
    ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(tailleDeCaractere);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
    ecranOLED.setCursor(0, 0);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
   ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);   // Couleur du texte, et couleur du fond 
    pinMode(SW,INPUT_PULLUP);
}
}
void loop() {
data_envoi();
envoi();
reception();
affichage_test();                            
delay(200);
ecranOLED.clearDisplay();

if (Serial.available()){                  
Serial.write(BTSerial.read());            //voir les tram envoyées et recu sur le terminale 

}
} 