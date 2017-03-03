#include <RTClib.h>
#include "EEPROM.h"
#include "cc1101.h"

RTC_Millis rtc;

//constantes pour le capteur de courant
const int pinOut = A0;
const int sensibilite = 100; //mmv/A
const int offset = 2500; // Vcc/2 en mV


//lampe
const int LAMPE = 7;
boolean etat_lampe = false;

//creation d'un objet cc1101
CC1101 cc1101;

byte syncWord[2] = {199, 0}; // mot de synchronisation

CCPACKET paquet; // le paquet envoye (debut de trame | syncword | donnees utiles | FCS)

// a flag that a wireless packet has been received
boolean packetAvailable = false;

//datenow
DateTime today;

void setup()
{
  Serial.begin(9600);

  //lampe
  pinMode(LAMPE, OUTPUT);

  //initialisation RTC
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("Initialisarion RTC terminee...");
  delay(100);

  // initialisation de l'antenne RF
  cc1101.init();
  cc1101.set_433_GFSK_500_K();        //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds avec frequence 433 Mhz)
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("Initialisarion antenne RF terminee...");
  
}

void loop()
{

   today = rtc.now();
   String date_now = getDate();
   String res;

  int heure = today.hour();
  if(heure >= 19 || heure < 7){
  //if(heure >= 9 && heure < 10 && today.minute()>11){
    //allumer la lampe
    if(etat_lampe == false){
      digitalWrite(LAMPE, HIGH);
       etat_lampe = true;
   }
   
   Serial.println("Lampe allume "+date_now);

  }else{

    //etteindre la lampe
    if(etat_lampe == true){
      digitalWrite(LAMPE, LOW);
      etat_lampe= false;
    }

    //envoyer par RF
    float sensor = getSensorValue();
    res = String(sensor, DEC);
    res += " "+date_now;
    Serial.println("mesure: "+res);
    formatPaquet(res);
    
  }
  
  delay(2000); // Attendre 2s
}



/**
 * récpeurer la mesure de courant entre 7h et 19h
 */

float getSensorValue(){

    int lu = analogRead(pinOut);
    
    float voltage = 5000*(lu/1024.0) + 15;
    
    float y = offset-voltage; //en mV
    
    float value = y/sensibilite;
  
    return value;
  
}


/**
 * Récpeurer la date et l'heure
 */
String getDate(){
   String datte ="";
   datte+=today.day();
   datte+='-';
   datte+=String(today.month(), DEC);
   datte+='-';
   datte+=String(today.year(), DEC);
   datte+=" ";
   datte+=String(today.hour(), DEC);
   datte+=':';
   datte+=String(today.minute(), DEC);
   datte+=':';
   datte+=String(today.second(), DEC);
   //date+='-';
   return datte;
   
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
    Serial.print("Taille ");
    Serial.println(message.length());
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




