#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>
#include <stdio.h>
#include <HCSR04.h>
#include <SoftwareSerial.h>

#include <MFRC522.h>
#include <avr/pgmspace.h>
#include <avr/io.h>    /* Include AVR std. library file */
#include <stdio.h>    /* Include std. library file */
#include <util/delay.h>   /* Include Delay header file */

//Definition des pin moteurs
#define enA 12    
#define in2 A0
#define in1 A1
#define enB 13
#define in4 A2
#define in3 A3
//definition Pin buzzer et Servomoteur
#define Pinservo 3
#define buzz A6

#define RST_PIN 0         // pin RFID
#define SS_PIN 4         // pin   RFID

//pin led

#define LedRfid 11 //ledRFID    
#define LedComm 10 //ledcommunication
#define LedAuto 21 //led mode auto
#define LedManu A7 //led mode manu
#define ledmoteurA 22
#define ledmoteurB 23

// stockage de chaine de caractere dans la memoire fixe en raison du manque d'espace dans la SRAM
const char string_angle[] PROGMEM = "Angle"; // "String 0" etc are strings to store - change to suit.
const char string_distence[] PROGMEM = "Distance";
const char string_modeA[] PROGMEM = "Mode Automatque";
const char string_modeM[] PROGMEM = "Mode mManuel";

// Table de string.
const char *const string_table[] PROGMEM = {string_angle, string_distence, string_modeA, string_modeM};

// declaration des differentes variables global

int RFID_OK = 0 ;
int Valeur_x = 511;
int Valeur_y = 511; 
int bouton = 0;
int SvaleurX = 511;
int SvaleurY= 0;
int Switch = 1;
int seuil_dist = 30;                         //distance max que le robot peut avoir avec l'obstacle
int distance = 0;                              //distance donnée par le sonnar
int memo_dist = 0;
int Vit_a = 0;
int Vit_b = 0;


unsigned long currentTime=0;
unsigned long delayrobot=0;
unsigned long delayultrason=0;
// definition des parametre de l'oled

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

//definition des differents modules de notre robot

#define triggerPin 15
#define echoPin 20
#define tx 1
#define rx 2
SoftwareSerial BTSerial(rx, tx); 
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin); 
Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);


MFRC522 mfrc522(SS_PIN, RST_PIN);           // crée notre instance du rfid

/////////////FONCTION 
void servo(int n){
     digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  for (int i = 0; i < 50; i++) {
    digitalWrite(4,HIGH);
    digitalWrite(Pinservo, HIGH);
    delayMicroseconds(n);
    digitalWrite(Pinservo, LOW);
    delayMicroseconds(20000-n);
  }//0º
  Switch = 1;

 }

void deplacement(){
   digitalWrite(Pinservo, LOW);
   digitalWrite(ledmoteurA,LOW);
   digitalWrite(ledmoteurB,LOW);
  //Partie gestion des vitesses de deplacement en mode manuel
if (Valeur_x > 490 && Valeur_x < 525 && Valeur_y > 495 && Valeur_y < 535){    
     digitalWrite(in1, LOW);
     digitalWrite(in2, LOW);
     digitalWrite(in3, LOW);
     digitalWrite(in4, LOW);
      Vit_a = 0;
      Vit_b = 0;
         }
        
else if(Valeur_y<470){
    // Moteur A Avant
     digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Moteur B avant
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);

     if (Valeur_x<300){
         Vit_a = map(Valeur_y, 470, 0, 0, 254);
         Vit_b = map(Valeur_x, 300, 0, 0, 200);
         }
  
     else if (Valeur_x>750){
         Vit_a = map(Valeur_x  ,80,1023,0,254);
         Vit_b = map(Valeur_y, 470, 0, 0, 180); 
         }
    
     else{
         Vit_a = map(Valeur_y, 470, 0, 0, 254);
         Vit_b = map(Valeur_y, 470, 0, 0, 254);
         }

  }

 else if (Valeur_y>550){

     // Moteur A Arriere
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      // Moteur B Arriere
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);

       if (Valeur_x<300){
    
         Vit_a = map(Valeur_y, 550, 1023, 0, 254);
         Vit_b = map(Valeur_x ,0,300,0,180);
        }
    
       else if (Valeur_x>750){
    
         Vit_a = map(Valeur_x, 550, 1023, 0, 254);
         Vit_b = map(Valeur_y, 550, 1023, 0, 187);  
        }
    
       else{
    
         Vit_a = map(Valeur_y, 550, 1023, 0, 254);
         Vit_b = map(Valeur_y, 550, 1023, 0, 254);
        }
 }

 else{

   if (Valeur_x<470){//GAUCHE
       // Moteur A Arriere
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      // Moteur B avant
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
     digitalWrite(ledmoteurA,HIGH);
     Vit_a = map(Valeur_x, 550, 1023, 0, 254);
     Vit_b = map(Valeur_x, 550, 1023, 0, 195);
  
     }

   else if (Valeur_x>550){//DROITE
      // Moteur A Avant
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Moteur B Arriere
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW); 
      Vit_a = map(Valeur_x, 550, 1023, 0, 195);
      Vit_b = map(Valeur_x, 550, 1023, 0, 254);
      digitalWrite(ledmoteurB,HIGH);
      }
 }
 analogWrite(enA, Vit_a); // le signal PWM A au moteur a 
 analogWrite(enB, Vit_b); // le signal PWM B au moteur b
}

void avancer(){               //fonction pour avancer
    
    // Moteur A Avant
    digitalWrite(ledmoteurA, HIGH);
    digitalWrite(ledmoteurB, LOW);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Moteur B avant
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
     
    //digitalWrite(ledmoteurA, HIGH);
    
    analogWrite(enA,160);   // le signal PWM A au moteur a 
    analogWrite(enB, 160);   // le signal PWM B au moteur b
    delay(1500);
    digitalWrite(ledmoteurA, LOW);
    //digitalWrite(ledmoteurB, LOW);
}

void reculer(){              //fonction pour reculer
   digitalWrite(ledmoteurB, HIGH);
   digitalWrite(ledmoteurA, LOW);
   // Moteur A Arriere
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Moteur B Arriere
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    analogWrite(enA, 160);    // le signal PWM A au moteur a 
    analogWrite(enB, 160);    // le signal PWM B au moteur b
    delay(1500);
    analogWrite(enA, 0);      // le signal PWM A au moteur a 
    analogWrite(enB, 0);      // le signal PWM B au moteur b
    digitalWrite(ledmoteurB, LOW);
}

void Turn_gauche(){           //fonction pour tourner a gauche de 90°
    // Moteur A Arriere
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Moteur B avant
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    //digitalWrite(ledmoteurA, HIGH); 

    analogWrite(enA, 127);   // le signal PWM A au moteur a 
    analogWrite(enB, 254);   // le signal PWM B au moteur b
    delay(1000);
    analogWrite(enA, 0);     // le signal PWM A au moteur a 
    analogWrite(enB, 0);     // le signal PWM B au moteur b
    //digitalWrite(ledmoteurA, LOW);
}

void Turn_droite(){          //fonction pour tourner à droite de 90°

    // Moteur A Avant
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Moteur B Arriere
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    //digitalWrite(ledmoteurB, HIGH); 

    analogWrite(enA, 254);    // le signal PWM A au moteur a 
    analogWrite(enB, 127);    // le signal PWM B au moteur b
   
    delay(1000);
    analogWrite(enA, 0);      // le signal PWM A au moteur a 
    analogWrite(enB, 0);      // le signal PWM B au moteur b
    //digitalWrite(ledmoteurB, LOW);

}

void check_distance(){        //fonction pour chercker la distance avec le capteur
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
  distance = distanceSensor.measureDistanceCm();
}

 void check_distance_gauche(){  //fonction pour vérifier distance avec sonnar a gauche
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
   servo(1900);  //pos 0

  delay(400);
  distance = distanceSensor.measureDistanceCm();
  delay(200);
   servo(1050);  //pos 0
  
}

void check_distance_droite(){   //fonction pour vérifier la distance avec sonnar à droite
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  servo(400); // 90 
  delay(200);
  distance = distanceSensor.measureDistanceCm();
  delay(200);
  servo(1050);  //pos 0
  
}

void setup() {
  
  //Initialisation du RFID
  SPI.begin();
  mfrc522.PCD_Init();       
  //Initialisation de la communication Serial
  BTSerial.begin(9600);
  Serial.begin(9600);


    //Definition des pins en OUTPUT pour les LED 
  pinMode(LedRfid,OUTPUT);
  pinMode(LedComm,OUTPUT);
  pinMode(LedAuto,OUTPUT);
  pinMode(LedManu,OUTPUT);
  pinMode(Pinservo,OUTPUT);
  pinMode(ledmoteurA,OUTPUT);
  pinMode(ledmoteurB,OUTPUT);

  pinMode(buzz,OUTPUT);
 
  // Definition des pin du moteur en sortie
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // TEST LED 
   digitalWrite(LedComm, HIGH);
   digitalWrite(LedRfid, HIGH);
   digitalWrite(ledmoteurA, HIGH);
   digitalWrite(ledmoteurB, HIGH);
   digitalWrite(LedAuto, HIGH);
   digitalWrite(LedManu, HIGH);
   digitalWrite(buzz, HIGH);
   delay(500);
   digitalWrite(LedComm, LOW);
   digitalWrite(LedRfid, LOW);
   digitalWrite(ledmoteurA, LOW);
   digitalWrite(ledmoteurB, LOW);
   digitalWrite(LedAuto, LOW);
   digitalWrite(LedManu, LOW);
   digitalWrite(buzz, LOW);
   servo(1050);  //pos 0

  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                                                 // Arrêt du programme (boucle infinie) si échec d'initialisation
      ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
      ecranOLED.setTextSize(1);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
      ecranOLED.setCursor(0, 0);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
      ecranOLED.setTextColor(SSD1306_WHITE, SSD1306_BLACK);       // Couleur du texte, et couleur du fond 
  ecranOLED.setCursor(0,20);  
  ecranOLED.print("OLED FONCTIONEL");
  ecranOLED.display();
  delay(2000);

  // Definition 
 //myservo.attach(servo); 
} 


void affichage(){
  // const char *const string_table[] PROGMEM = {string_angle, string_distence, string_modeA, string_modeM};
  //Affichage des informations sur le OLED
  char buffer_Oled[30];
  char buffer_text[40];
  ecranOLED.clearDisplay();

  ecranOLED.setCursor(0, 10);  
  sprintf(buffer_Oled,"X1:%i |Y1:%i",Valeur_x,Valeur_y);                               
  ecranOLED.print(buffer_Oled);
  
  ecranOLED.setCursor(0,20);
  buffer_text[strlen_P(string_table[1])];
  strcpy_P(buffer_text, (PGM_P)pgm_read_word(&string_table[1]));
  sprintf(buffer_Oled,"%s: %icm ",buffer_text,distance);                               
  ecranOLED.print(buffer_Oled); 

  ecranOLED.setCursor(0,30);
  sprintf(buffer_Oled,"J2 X:%d y:%d",SvaleurX,SvaleurY);                          
  ecranOLED.print(buffer_Oled);  

  ecranOLED.setCursor(0,40);
  int angle = map(SvaleurX,0,1023,0,180);
  buffer_text[strlen_P(string_table[0])];
  strcpy_P(buffer_text, (PGM_P)pgm_read_word(&string_table[0]));
  sprintf(buffer_Oled,"%s:%i ",buffer_text,angle);                                
  ecranOLED.print(buffer_Oled);                               

  ecranOLED.setCursor(0,0);
 if ( bouton == 0){
  buffer_text[strlen_P(string_table[3])];
  strcpy_P(buffer_text, (PGM_P)pgm_read_word(&string_table[3]));
  ecranOLED.print(buffer_text);}

 else {
  buffer_text[strlen_P(string_table[2])];
   strcpy_P(buffer_text, (PGM_P)pgm_read_word(&string_table[2]));
   ecranOLED.print(buffer_text);}
  
ecranOLED.display();
}
//Code de reception de la communication Serie
void reception(){
  char buffer_serie[33];
  //char debogage[31];
  digitalWrite(LedComm, HIGH); 

  BTSerial.readBytesUntil('>',buffer_serie,33);// essayer avec until apres
  char *M_Valeurx = strtok(buffer_serie,"AB");
  Valeur_x = atoi(M_Valeurx);
  char *M_Valeury = strtok(0,"CD");
  Valeur_y = atoi(M_Valeury);
  char *M_Bouton = strtok(0,"EF");
  bouton = atoi(M_Bouton);
  char *M_SValeurx = strtok(0,"GH");
  SvaleurX = atoi(M_SValeurx);
  char *M_SValeury = strtok(0,"IJ");
  SvaleurY= atoi(M_SValeury);
  char *M_Switch = strtok(0,"KL");
  Switch= atoi(M_Switch);
  BTSerial.flush();
  digitalWrite(LedComm, LOW); 
}

void envoi(){
   //Ecriture de la communication
   char buffer_envoi[10];
   sprintf(buffer_envoi,"<A%dB>",memo_dist);  
   BTSerial.write(buffer_envoi);
}

bool enregistrer_check_rfid(){
// Enregistrer l'ID du badge (4 octets) 
  byte nuidPICC[4];
  byte NotreBadge[4] = {0x6C,0x62,0xEF,0x37}; // code de notre badge
  byte MichaelBadge[4] = {0x9C,0xF5,0x37,0x63}; // Code du porte clé
  byte MichaelCarte[4] = {0x93,0x78,0xA2,0x4}; // Code de la carte
  byte i = 0;
  if(RFID_OK != 1){
      for (i = 0; i < 4; i++) {
        nuidPICC[i] = mfrc522.uid.uidByte[i];
      }
      
      for (i = 0; i < 4; i++)  {                             //on vérifie si le code correspond à celui de notre badge

        if ( nuidPICC[i] != MichaelBadge[i] ){
          break;
        }
      
   else if ((i == 3) && (nuidPICC[i] == MichaelBadge[i])){  
        RFID_OK =1;  
        digitalWrite(LedRfid, HIGH);       
        return true;   
    }
  }
 }
 else{ return true;}
}

void loop() {
  currentTime = millis();
  if (BTSerial.available()){
    char incomingByte = BTSerial.read(); 
    if (incomingByte == '<'){ 
        reception();
        affichage();
       }
  }
  if ((currentTime - delayultrason) > 500){
    check_distance();
    delayultrason = currentTime;
    }
  int result = abs(memo_dist - distance);
  if (result >= 15){  
    memo_dist = distance;
    affichage();
    envoi();
    }

 
if (distance < seuil_dist && distance !=-1){
      tone(buzz,50, 600);           // allume le buzzer actif arduino
    }
 
 if(bouton == 0 ){// le true est pour debuger probleme avec les bouton
    //Mode Manuel
   //Actualisation des LEDS
    RFID_OK =0;
    digitalWrite(LedManu, HIGH);
    digitalWrite(LedAuto, LOW);
    
    deplacement();
    
    //Controle du Servomoteur
    
    if( Switch==0){servo(map(SvaleurX,0,1023,480,2000)); 
    }
    //Gestion du buzzer
 }

  if(bouton == 1 ){
    //Mode Automatique
    digitalWrite(LedManu, LOW);
    digitalWrite(LedAuto, HIGH);
    
    // Initialisé la boucle si aucun badge n'est présent 
    if ( !mfrc522.PICC_IsNewCardPresent() && RFID_OK != 1)
     {return;}

  // Vérifier la présence d'un nouveau badge 
    if ( !mfrc522.PICC_ReadCardSerial()&& RFID_OK != 1)
      {return;}
    
    if (RFID_OK ==1){       //si on a pas check avec le bon badge une fois  alors le robot fonctionne pas
      digitalWrite(LedRfid,HIGH);  
      check_distance();
      
       
      
        if (distance > seuil_dist){
          avancer();}                 //si on a un objet trop proche, on revérifie d'abord 
        
        else if(seuil_dist >= distance){
          check_distance_gauche();
           if (seuil_dist < distance){      //si pas d'obstacle à gauche
              Turn_gauche();}             //on tourne les roues vers la gauches
           if (seuil_dist < distance){ {
              check_distance_droite(); } 
           if (seuil_dist < distance){      //si c'est bon alors on tourne a droite
              Turn_droite();}

          else{
              reculer();
                }
            }     
     
    }

 }
 else {enregistrer_check_rfid();
  servo(1050);  //pos 0
  }
}
}

