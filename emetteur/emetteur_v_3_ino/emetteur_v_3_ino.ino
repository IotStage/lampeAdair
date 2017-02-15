
#include "EEPROM.h"
#include "cc1101.h"



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

int heure=10;
long times=0;
long delai_envoi = 0;


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
  times = millis();
  delai_envoi=millis();  
}

void loop()
{
   

  if(heure >= 20 || heure < 6){
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

    if(heure >= 10 && heure <= 16)
    {
      if(millis() - delai_envoi > 300000)
      //envoyer par RF
      {
        String res;
        float sensor = getSensorValue();
        res = String(sensor, DEC);
        Serial.println("mesure: "+res);
        res+=" "+String(heure, DEC); // on ajoute l'heure ur la mesure a envoyer
        formatPaquet(res);
        delai_envoi=millis();
      }
    } 
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

int getHeure()
{
  
  if(millis()-times > 3600000 )
  {
    times = millis();
    
    //heure = heure == 23 ? heure+1 : 0    
    if(heure == 23 ) 
      heure = 0;
    else 
      heure +=1; 
  }
  return heure;
}


