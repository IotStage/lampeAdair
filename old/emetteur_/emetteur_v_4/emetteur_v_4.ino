
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

long delai_envoi = 0;

//constantes pour la date
int mois_;
int jour_;
int an_;
int heure_=0;
int minute_=0;
int seconde_ = 0;
long date_depart;


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
  initialisation_jour_heure();
  delai_envoi=millis();  
}

void loop()
{
   determination_date();

  if(heure_ >= 20 || heure_ < 6){
    //allumer la lampe
    if(etat_lampe == false){
      digitalWrite(LAMPE, HIGH);
       etat_lampe = true;
   }
   
   Serial.println("Lampe allume a l'heure "+heure_);

  }
  else{

    //etteindre la lampe
    if(etat_lampe == true){
      digitalWrite(LAMPE, LOW);
      etat_lampe= false;
    }

    if(heure_ >= 11 && heure_ < 16)
    {
      
      if(millis() - delai_envoi > 30000)
      //envoyer par RF
      {
        String res;
        float sensor = getSensorValue();
        res = String(sensor, DEC);
        res+=" "+String(heure_)+":"+String(minute_)+":"+String(seconde_); // on ajoute l'heure ur la mesure a envoyer
        formatPaquet(res);
        Serial.println("mesure: "+res);
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



void determination_date(){//fonction nécessaire si on ne dispose pas d'un module timing extérieur
   boolean changt_date = false;
   long date_presente = millis();
   long temps_ecoule;
   if(date_presente < date_depart){
      temps_ecoule=date_presente-date_depart;
   }
   else{
     
   }
   
   temps_ecoule=temps_ecoule/1000;
   if ((seconde_+temps_ecoule)<=59){
    seconde_ = seconde_+temps_ecoule;
   }
   else{
    seconde_=(seconde_+temps_ecoule)-60;
    if (minute_<59){minute_=minute_+1;}
    else{minute_=0;
      if(heure_<23){heure_=heure_+1;}
      else{heure_=0;
        switch (mois_) {
   case 1 :if (jour_<31){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 2;}
   break;
   case 2 :if (jour_<28){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 3;}
   break;
   case 3 :if (jour_<31){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 4;}
   break;
   case 4 :if (jour_<30){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 5;}
   break;
   case 5 :if (jour_<31){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 6;}
   break;
   case 6 :if (jour_<30){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 7;}
   break;
   case 7 :if (jour_<31){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 8;}
   break;
   case 8 :if (jour_<31){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 9;}
   break;
   case 9 :if (jour_<30){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 10;}
   break;
   case 10 :if (jour_<31){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 11;}
    break;
   case 11 :if (jour_<30){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 12;}
   break;
   case 12 :if (jour_<31){jour_=jour_+1;}
   else{jour_ = 1;
   mois_= 1;
   an_= an_+1;}
   break;
   }
   }
   }
   }
   date_depart=millis();
}

void initialisation_jour_heure(){//fonction nécessaire si on ne dispose pas d'un module timing extérieur
  an_=17;
  mois_=2;
  jour_=17;
  heure_=17;
  minute_=0;
  seconde_=0;
}



