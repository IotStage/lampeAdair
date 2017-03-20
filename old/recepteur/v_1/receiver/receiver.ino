#include "EEPROM.h"
#include "cc1101.h"
#include <Ethernet.h>

//les SS du bus spi
const int SS_ETHERNET = 9;
const int SS_RF = 10;

//creation d'un objet cc1101
CC1101 cc1101;

byte syncWord[2] = {199, 0};

// a flag that a wireless packet has been received
boolean packetAvailable = false;

// parametres shield ethernet
IPAddress ip(192,168,1,2);
EthernetClient client;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server [] = "192.168.1.101";

byte address = 0x00;


/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}

void setup()
{
  Serial.begin(9600);

  initialisation();
  
  pinMode(SS_ETHERNET, OUTPUT);
  //pinMode(SS_RF, OUTPUT);

  //activer le rf et desactiver lethernet
  digitalWrite(SS_ETHERNET, HIGH);
  //digitalWrite(SS_RF, LOW);

  //SPI.begin(); // wake up the SPI bus.
  //SPI.setBitOrder(MSBFIRST);
  
  
  
}


void loop()
{
  verificationPaquetRecu();
  //sendEthernetData("16.78");
}


/**
 * Initialiser les modules
 */
void initialisation(){

  Serial.println("Intialisation...");

  //initialisation du module ethernet
  if(!Ethernet.begin(mac)){
    Ethernet.begin(mac, ip);
    Serial.print("parametrage avec ip fixe: ");
    Serial.println(ip);
  }else{
    Serial.println("parametrage avec dhcp");
  }
  Serial.println("ethernet initialisé");
  
  // initialisation de l'antenne RF
  cc1101.init();

  cc1101.set_433_GFSK_500_K();        //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds avec frequence 433 Mhz)
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
  
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("RF initialise");

}

/**
 * Récupérer le paquet
 */
String receptionPaquet(){
  CCPACKET paquet;
  String data="";
  
    
    if(cc1101.receiveData(&paquet) > 0){
      if(!paquet.crc_ok) {
        Serial.println("Erreur CRC");
      }
      if(paquet.length > 0){
        if(paquet.length == 60 && paquet.data[55]=='~' && paquet.data[56]=='@' && paquet.data[57]==']' && paquet.data[58]=='#' && paquet.data[59]=='`' ){
          char donnees [56];
          for(int j=0; j<55; j++){
            Serial.write(paquet.data[j]);
            data+=paquet.data[j];
            donnees[j]= (char) paquet.data[j];
            
          }
          donnees[55]='\0';
          sendEthernetData(donnees);
        }
        else{
          char donnees [paquet.length+1];
          for(int j=0; j<paquet.length; j++){
            Serial.write(paquet.data[j]);
            data+=paquet.data[j];
            donnees[j]= (char) paquet.data[j];
          }
          donnees[paquet.length]='\0';
          sendEthernetData(donnees);
          Serial.println("");
          
        }
      }
    }
    
    return data;
}

/**
 * Verifier s'il y'a un paquet à recevoir et le récuperer s'il y'en a
 */
void verificationPaquetRecu(){
  if(packetAvailable){
    //Serial.println("Paquet recu");
    
    // Desactivation de la reception
    detachInterrupt(0);
    packetAvailable = false;
    receptionPaquet();

    // Activation de la reception
    attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  }
}


/**
 * Envoyer des données sur le réseau ethernet
 */
void sendEthernetData(String donnees){

  //formatter les donnees
  donnees.replace(" ", "%20");
  
  //se connecter
  client.connect(server, 80);
  Serial.println("Connexion...");

  //envoyer les donnees
  Serial.println("envoi des donnees:\n---------------------------------------");
  Serial.println(donnees);
  Serial.println("---------------------------------------------");
  
  client.print("GET /lampeadair.php?donnees=");
  client.print(donnees);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: keep-alive");
  client.println();

   //se deconnecter
  Serial.println("Deconnexion...");
  client.stop();
}


