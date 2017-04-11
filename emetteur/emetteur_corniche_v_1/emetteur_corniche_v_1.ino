
#include "EEPROM.h"
#include "cc1101.h"

 //constantes tension

const float sensibiliteTension = 0.0681*1000;
const float offsetTension = 2500.0;
//const int voltageSensorPin = A1;

//constantes pour le capteur de courant
const int pinOut = A0;
const int sensibilite = 100; //mmv/A
const int offset = 2500; // Vcc/2 en mV

const int id=2;

float vout = 0.0;
float vin = 0.0;
float R1 = 10000; //10k
float R2 = 1000; //1000 ohm resistor, I tweaked this
int val_lu = 0;
int analogInputPanneau = A1; // I used A1
int analogInputBatterie = A2;

//lampe
const int LAMPE = 7;
boolean etat_lampe = false;
boolean etat_batterie = true; //pour verifier si on peut allumer la lampe ou pas
boolean jour = false;
boolean ALLUMER = true;
boolean ETEINDRE = false;

//creation d'un objet cc1101
CC1101 cc1101;

byte syncWord[2] = {199, 0}; // mot de synchronisation

CCPACKET paquet; // le paquet envoye (debut de trame | syncword | donnees utiles | FCS)

boolean packetAvailable = false;

int heure=19;
unsigned long times=0;
unsigned long delai_envoi = 0;
unsigned long MAX_ULONG = 4294967295L;
unsigned long temp;

long DELAI_ENVOI_MATIN = 50000; //60;
long DELAI_ENVOI_SOIR = 1700000; //30*DELAI_ENVOI_MATIN
int HEURE_LAMPE_DEBUT = 19;
int HEURE_LAMPE_FIN = 6;

float precedent=0.0;
float SEUIL = 12.0;

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
  precedent = getTensionBatterie();
}

void loop()
{
   getHeure();
   //Serial.println(heure);
  //if()
   

  if(heure >= HEURE_LAMPE_DEBUT || heure < HEURE_LAMPE_FIN){
    allumerLampe();
    
    temp = millis();
    if(temp - delai_envoi >= DELAI_ENVOI_SOIR){ //
      etat_batterie = getEtatBatterie();
      envoiDonnees();
      delai_envoi=temp;
    }
  }
  else{
    
    eteindreLampe();
    //if(heure >= 11 && heure < 16){
      temp = millis();
      if(temp - delai_envoi >= DELAI_ENVOI_MATIN){
        envoiDonnees();
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

float getTension(int pin){
   val_lu = analogRead(pin);
   vout = (val_lu * 5.0) / 1024.0;
   vin = vout / (R2/(R1+R2)); 
   return vin;
}

float getTensionBatterie(){
    return getTension(analogInputPanneau);
}

float getTensionPanneau(){
  int sensor = analogRead(analogInputBatterie);
  float voltage = sensor*(5000.0/1023.0);
  float num = voltage - offsetTension;
  float mesure = num / sensibiliteTension; //-3.0;
  
  return mesure;
   //return getTension(analogInputBatterie);
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
/*
 * cette fonction permet de verifier l'etat de la betterie pour l'allumage de la lampe
 * Dans cette fonction on garde toujours la valeur precedente de la tension de la batterie
 * si :
 *    SEUIL < present < precedent => ollume la lampe 
 *    SEUIL < precedent < present => ollume la lampe 
 *    present < SEUIL < precedent => ollume la lampe 
 *    precedent < SEUIL < present => ollume la lampe 
 *    present < precedent < SEUIL => eteind la lampe 
 *    precedent < present < SEUIL => eteind la lampe 
 *    
 *    
 */
// 

boolean getEtatBatterie(){
  float present = getTensionBatterie();
  
  if(SEUIL < present && present < precedent){
    precedent=present;
    return ALLUMER;
  }
  if(SEUIL < precedent && precedent< present){
    precedent=present;
    return ALLUMER;
  }
  if(present < SEUIL && SEUIL < precedent){
    precedent=present;
    return ALLUMER;
  } 
  if(precedent < SEUIL && SEUIL < present){
    precedent=present;
    return ALLUMER;
  }
  if(present < precedent && precedent < SEUIL){
    precedent=present;
    return ETEINDRE;
  }
  if(precedent < present && present < SEUIL){
    precedent=present;
    return ETEINDRE;
  }  
     
  //return getTensionBatterie() > 12;
}

void envoiDonnees(){
  
  String res = String(id, DEC);
  float courant =   getSensorValue();
  float tension2 =  getTensionPanneau();
   float tension1 = getTensionBatterie();
  res +=" "+String(courant, 3);
  res +=" "+String(tension1, 3);
  res +=" "+String(tension2, 3);
  res+=" "+String(heure, DEC); // on ajoute l'heure ur la mesure a envoyer
  res+=" "+String(getEtatLampe());
  formatPaquet(res);
  Serial.println("mesure: "+res);
}

boolean getEtatLampe(){
  return etat_lampe == true && jour == false;
}

void allumerLampe(){
  
    //allumer la lampe
   if(etat_lampe == false && etat_batterie == true && jour == false){
      digitalWrite(LAMPE, HIGH);
       etat_lampe = true;
       envoiDonnees();
   }else if(etat_batterie == false){
     digitalWrite(LAMPE, LOW);
     jour = true;
     envoiDonnees();
   }
}

void eteindreLampe(){
  jour = false;
    //etteindre la lampe
    if(etat_lampe == true){// || 
      digitalWrite(LAMPE, LOW);
      etat_lampe= false;
      envoiDonnees();
   }
}

