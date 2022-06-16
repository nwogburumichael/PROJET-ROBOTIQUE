#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>
#include <avr/pgmspace.h>


#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé du MEGA (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

#define bout_mode 15
#define BSER A2
#define Vry A7
#define SW 14
#define Vrx  A6
#define SERVx A3 
#define SERVy A4
#define LED_ON 23
#define LED_MANU 21
#define LED_AUTO 22
#define LED_COMM 20
#define BUZZER 13
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);
// memo valeur
int memo_servx = 0;
int memo_servy = 0;
int memo_vrx = 0;
int memo_vry = 0;
int memo_bser = 0; // besoin d'un flag
int memo_sw = 0; // besoin d'un flag
int memo_mode = 0;


const char string_modeA[] PROGMEM = "Mode Automatque";
const char string_modeM[] PROGMEM = "Mode mManuel";

// Table de string.
const char *const string_table[] PROGMEM = {string_modeA, string_modeM};

byte tailleDeCaractere=1;

int man_x = 0, man_y = 0, ValSX = 0, ValSY = 0, distance = 0;
int val_joy2 = 0,val_mode =0,val_joy1 = 0;


SoftwareSerial BTSerial(2, 1);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(SW, INPUT_PULLUP);
  pinMode(BSER, INPUT_PULLUP);
  pinMode(bout_mode, INPUT_PULLUP);
  pinMode(LED_ON,OUTPUT);
  pinMode(LED_MANU,OUTPUT);
  pinMode(LED_AUTO,OUTPUT);
  pinMode(LED_COMM,OUTPUT);
  pinMode(BUZZER,OUTPUT);

  
   // TEST
  digitalWrite(LED_ON,HIGH);
  digitalWrite(LED_MANU,HIGH);
  digitalWrite(LED_COMM,HIGH);
  digitalWrite(LED_AUTO,HIGH);
  //digitalWrite(BUZZER,HIGH);
  delay(2000);
  digitalWrite(LED_ON,LOW);
  digitalWrite(LED_MANU,LOW);
  digitalWrite(LED_COMM,LOW);
  digitalWrite(LED_AUTO,LOW);
  //digitalWrite(BUZZER,LOW);


  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                                                     // Arrêt du programme (boucle infinie) si échec d'initialisation
      ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
      ecranOLED.setTextSize(tailleDeCaractere);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
      ecranOLED.setCursor(0, 0);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);       // Couleur du texte, et couleur du fond 
  ecranOLED.setCursor(0,20);  
  ecranOLED.print("OLED FONCTIONEL");
  Serial.print("OLED FONCTIONEL");
  ecranOLED.display();
  delay(3000);   
}
bool difference(){
  char test[40];

  if (memo_bser != val_joy2|| memo_sw != val_joy1 || memo_mode != val_mode){
    memo_bser = val_joy2; // besoin d'un flag
    memo_sw = val_joy1; // besoin d'un flag
    memo_mode = val_mode;
    return true;
  }
  if (abs(memo_servx - ValSX) > 60 || abs(memo_servy - ValSY) > 60 || abs(memo_vrx - man_x) > 60 || abs(memo_vry - man_y) > 60){// TEST AVEC IFF
     memo_servx = ValSX;
     memo_servy = ValSY;
     memo_vrx = man_x;
     memo_vry = man_y;
    return true;
  }
  else {return 0;}
}
void reception(){     //fonction pour la reception
  digitalWrite(LED_COMM,HIGH);
  char buf[20];

  BTSerial.readBytesUntil('>',buf,6);
  char *R_distance = strtok(buf,"AB");
  distance = atoi(R_distance);
  digitalWrite(LED_COMM,LOW);
  }

void envoi(){                                      //fonction envoi au robot
  char tram[40];
  digitalWrite(LED_COMM,HIGH);
  sprintf(tram ,"<A%04dBC%04dDE%dFG%04dHI%04dJK%dL>",man_x,man_y,val_mode,ValSX,ValSY,memo_bser);  //tram qu'on envoi
  BTSerial.write(tram);
  digitalWrite(LED_COMM,LOW);
  
}

void Actualisation_Variable(){           //fonction prise de donnée a envoyé
    man_x = analogRead(Vrx);
    man_y = analogRead(Vry);
    ValSX = analogRead(SERVx);
    ValSY = analogRead(SERVy);
    val_joy1 = digitalRead(SW);
    val_joy2 = digitalRead(BSER);
    val_mode = digitalRead(bout_mode);
    }


void affichage(){                          //fonction d'affichage sur l'OLED

  char buffer_Oled[60];
  char buffer_text[40];
  ecranOLED.clearDisplay();
 
  ecranOLED.setCursor(0, 10);  
  sprintf(buffer_Oled,"joy1 X1:%i |Y1:%i",man_x,man_y);                               
  ecranOLED.print(buffer_Oled);
  
  ecranOLED.setCursor(0, 20);  
  sprintf(buffer_Oled,"joy2 X1:%i |Y1:%i",ValSX,ValSY);                               
  ecranOLED.print(buffer_Oled);

  ecranOLED.setCursor(0,30);             //afficher la distance entre robot et obstacle
  sprintf(buffer_Oled,"distance : %d cm ", distance);
  ecranOLED.print(buffer_Oled);

  ecranOLED.setCursor(0,40);           
  sprintf(buffer_Oled,"BP1:%i BP2:%i BPmo:%i %i",val_joy1,val_joy2,val_mode,memo_mode);   
  ecranOLED.print(buffer_Oled);

  //PORBLEME 
   ecranOLED.setCursor(0,0); 
     
 if ( memo_mode == 0){
  buffer_text[strlen_P(string_table[0])];
  strcpy_P(buffer_text, (PGM_P)pgm_read_word(&string_table[0]));
  ecranOLED.print(buffer_text);
  digitalWrite(LED_MANU,LOW);
  digitalWrite(LED_AUTO,HIGH);
  }

 else {
  buffer_text[strlen_P(string_table[1])];
   strcpy_P(buffer_text, (PGM_P)pgm_read_word(&string_table[1]));
   ecranOLED.print(buffer_text);
    digitalWrite(LED_MANU,HIGH);
    digitalWrite(LED_AUTO,LOW);
   }
  

  ecranOLED.display();
}

void loop() {
   
Actualisation_Variable();

if (difference() == true){envoi();
        affichage();} 


if (BTSerial.available()){
    char incomingByte = BTSerial.read(); 
  if (incomingByte == '<'){ 
        reception();
        affichage();
       }
  }

/*if (distance <= 30 && distance > 20){                      //si un objet est trop proche, on active le buzzer
  analogWrite(BUZZER,80);

}
  else if(distance<=20 && distance > 10){
    analogWrite(BUZZER,110); 
  }
  else if (distance <= 10 && distance > 0){
    analogWrite(BUZZER,130);
  }    
  else {
analogWrite(BUZZER, 0); 
}*/
ecranOLED.clearDisplay();
}