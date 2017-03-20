
#include "EEPROM.h"
#include "cc1101.h"



//constantes pour le capteur de courant
const int pinOut = A0;
const int sensibilite = 100; //mmv/A
const int offset = 2500; // Vcc/2 en mV

const int id=1;

float vout = 0.0;
float vin = 0.0;
float R1 = 10000; //10k
float R2 = 1000; //1000 ohm resistor, I tweaked this
int val_lu = 0;
int analogInput = A1; // I used A1

//lampe
const int LAMPE = 7;
boolean etat_lampe = false;

//creation d'un objet cc1101
CC1101 cc1101;

byte syncWord[2] = {199, 0}; // mot de synchronisation

CCPACKET paquet; // le paquet envoye (debut de trame | syncword | donnees utiles | FCS)

// a flag that a wireless packet has been received
boolean packetAvailable = false;

int heure=19;
unsigned long times=0;
unsigned long delai_envoi = 0;
unsigned long MAX_ULONG = 4294967295L;
unsigned long temp;

int HEURE_LAMPE_DEBUT = 19;
int HEURE_LAMPE_FIN = 7;

void setup()
{
  Serial.begin(9600);
  
  //lampe
  pinMode(LAMPE, OUTPUT);
  
  // initialisation de l'antenne RF
  cc1101.init();
  cc1101.set_433_GFSK_500_K();        //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds avec frequence 433 Mhz)
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("Initialisarion antenne RF terminee...");

  //initilisation de lheure
  times = millis();
  delai_envoi=millis();  
}

void loop()
{
   getHeure();
   //Serial.println(heure);

  if(heure >= HEURE_LAMPE_DEBUT || heure < HEURE_LAMPE_FIN){
    //allumer la lampe
    if(etat_lampe == false){
      digitalWrite(LAMPE, HIGH);
       etat_lampe = true;
   }
   
   Serial.println("Lampe allume a l'heure "+heure);

  }
  else{

    //etteindre la lampe
    if(etat_lampe == true){
      digitalWrite(LAMPE, LOW);
      etat_lampe= false;
    }

    //if(heure >= 11 && heure < 16){
      temp = millis();
      if(temp - delai_envoi >= 30000){
        
        String res = String(id, DEC);
        float courant = getSensorValue();
        float tension = getTension();
        res +=" "+String(courant, DEC);
        res +=" "+String(tension, DEC);
        res+=" "+String(heure, DEC); // on ajoute l'heure ur la mesure a envoyer
        formatPaquet(res);
        Serial.println("mesure: "+res);
        delai_envoi=temp;
      }
    //} 
  }
  
  //delay(2000); // Attendre 2s
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

float getTension(){
   // read the value at analog input
   val_lu = analogRead(analogInput);
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

void getHeure()
{
  temp = millis();
  if(temp < times){
    if((MAX_ULONG-times+temp) >= 3600000){
      times = temp;
     if(heure == 23 ) 
        heure = 0;
      else 
        heure +=1;
    }
  }
  else{
    
    if(temp-times >= 3600000 )
    {
      times = temp; //+(temp-times-3600000)
      
      //heure = heure == 23 ? heure+1 : 0    
      if(heure == 23 ) 
        heure = 0;
      else 
        heure +=1; 
    }
  }
  //return heure;
}


