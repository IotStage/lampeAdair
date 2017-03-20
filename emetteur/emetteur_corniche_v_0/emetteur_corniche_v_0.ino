
#include "EEPROM.h"
#include "cc1101.h"

//*************** identifiant du dispositif
const int id=2;

//constantes pour le capteur de courant
const int pinOut = A0;
const int sensibilite = 110; //mmv/A
const int offset = 1650; // Vcc/2 en mV

//******************************capteurs de tensions
int INPUT_BATTERIE = A1; // I used A1
int INPUT_SOLAIRE = A2;
float vout = 0.0;
float vin = 0.0;
float R1 = 10000; //10k
float R2 = 1000; //1000 ohm resistor, I tweaked this
int val_lu = 0;
const float TENSION_SEUILLE_BATTERIE = 12.0;


//************************** lampe
const int LAMPE = 7;
boolean LAMPE_ALLUMEE = false; //renseigne sur letat de la lampe
boolean NORMALE_STATE = false; 
unsigned int HEURE_LAMPE_DEBUT = 19;
unsigned int HEURE_LAMPE_FIN = 6;

//************************************* RF
CC1101 cc1101;
byte syncWord[2] = {199, 0}; // mot de synchronisation
CCPACKET paquet; // le paquet envoye (debut de trame | syncword | donnees utiles | FCS)
boolean packetAvailable = false;


//********************* transmissions
int heure=10;
unsigned long times=0;
unsigned long last_envoi = 0;
unsigned long MAX_ULONG = 4294967295L;
unsigned long temp;
unsigned current_millis;
unsigned int PERIODE_ENVOIE = 25000;


void setup()
{
  Serial.begin(9600);
  
  //lampe
  pinMode(LAMPE, OUTPUT);
  pinMode(INPUT_BATTERIE, INPUT);
  pinMode(INPUT_SOLAIRE, INPUT);
  // initialisation de l'antenne RF
  cc1101.init();
  cc1101.set_433_GFSK_500_K();        //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds avec frequence 433 Mhz)
  //cc1101.setChannel(10);
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("Initialisarion antenne RF terminee...");

  //initilisation de lheure
  times = millis();
}

void loop()
{
    current_millis = millis();
    getHeure();

  //envoyer les données par RF
  if(current_millis - last_envoi > PERIODE_ENVOIE) { //envoyer par RF
      String res= String(id, DEC);
      float courant = getSensorValue();
      float tension_batterie = getTensionBatterie();
      float tension_solaire = getTensionSolaire();
      res +=" "+String(courant, DEC);
      res +=" "+String(tension_batterie, DEC);
      res +=" "+String(heure, DEC); // on ajoute l'heure ur la mesure a envoyer
      res +=" "+String(tension_solaire, DEC);
      //formatPaquet(res);
      Serial.println("mesure: "+res);
      last_envoi = current_millis;

      if(tension_batterie<TENSION_SEUILLE_BATTERIE){
          NORMALE_STATE = false;
      }else{
          NORMALE_STATE = true;
      }
  }

  if(heure >= HEURE_LAMPE_DEBUT || heure < HEURE_LAMPE_FIN){
     allumerLampe();
  }else{
    eteindreLampe();
  }
  //Serial.println("Lampe allume a l'heure "+LAMPE_ALLUMEE);
}



/**
 * récpeurer la mesure de courant entre 7h et 19h
 */

float getSensorValue(){
    int lu = analogRead(pinOut);
    float voltage = 5000*(lu/1024.0); // + 15;
    float y = offset-voltage; //en mV
    float value = y/sensibilite;
    return value;
  
}

float getTensionBatterie(){
   val_lu = analogRead(INPUT_BATTERIE);
   vout = (val_lu * 5.0) / 1024.0;
   vin = vout / (R2/(R1+R2)); 
   return vin;
}

float getTensionSolaire(){
   val_lu = analogRead(INPUT_SOLAIRE);
   vout = (val_lu * 5.0) / 1024.0;
   vin = vout / (R2/(R1+R2)); 
   return vin;
}


/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}


void envoiPaquet() {
  if(cc1101.sendData(paquet)){
    Serial.println("Envoi termine");
   }else{
    Serial.println("Echec de l'envoi");
  }
}

/***
 * format segmentatoin des donnees a envoyer en paquet, 
 * ajout d'une entete specifique (~@]#) pour limiter les paquets
 */
void formatPaquet(String message){
  if(message.length()<61){
    paquet.length=message.length();
    message.getBytes(paquet.data, message.length()+1);
    envoiPaquet();
  }
  else{
    String partie1 = message.substring(0, 55);
    String partie2 = message.substring(55);
    partie1+="~@]#`";
    formatPaquet(partie1);
    //delay(500);
    formatPaquet(partie2);
  }
}

void getHeure()
{
 
  if(current_millis < times){
    if((MAX_ULONG-times+current_millis) >= 3600000){
      times = current_millis;
     if(heure == 23 ) 
        heure = 0;
      else 
        heure +=1;
    }
  }
  else{
    
    if(current_millis-times >= 3600000 )
    {
      times = current_millis; //+(temp-times-3600000)
      
      //heure = heure == 23 ? heure+1 : 0    
      if(heure == 23 ) 
        heure = 0;
      else 
        heure +=1; 
    }
  }
}

void allumerLampe(){
  if(LAMPE_ALLUMEE == false && NORMALE_STATE == true){
       digitalWrite(LAMPE, HIGH);
       LAMPE_ALLUMEE = true;
       Serial.println("Lampe allumeer");
   }else if(NORMALE_STATE == false){
     eteindreLampe();
   }
}

void eteindreLampe(){
  if(LAMPE_ALLUMEE == true){
      digitalWrite(LAMPE, LOW);
      LAMPE_ALLUMEE = false;
      Serial.println("Lampe eteinte");
   }
}

