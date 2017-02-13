#include <RTClib.h>
#include <Wire.h>
#include <SD.h>

RTC_Millis rtc;

//constantes pour le capteur de courant
const int pinOut = A0;
const int sensibilite = 100; //mmv/A
const int offset = 2500; // Vcc/2 en mV


//lampe
const int LAMPE = 7;
boolean lampe_allumee = false;

void setup()
{
  Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("echec initialisation!");
    return;
  }
  
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  Wire.begin(); // Rejoindre le bus I2C (Pas besoin d adresse pour le maitre)
  delay(100);
  Serial.println("initialisarion terminee.");
}

void loop()
{
  float sensor = getSensorValue();
  String res = String(sensor, DEC);
  if(sensor != 0.0){
    sauvegarde(res);
    sendToSlave(res);
  }
  
  allumeLampe();
  delay(2000); // Attendre 2s
}

/**
 * envoyer les données à l'autre carte
 */
void sendToSlave(String res)
{
 
    char buff[res.length()];
    res.toCharArray(buff, res.length());
    Wire.beginTransmission(4); // Envoyer vers device #4
    Wire.write(buff); // Envoi un 1
    Wire.endTransmission();    // Arreter la transmission
    Serial.println("donnees envoyees au slave");
  
}

/**
 * récpeurer la mesure de courant entre 7h et 19h
 */

float getSensorValue(){

  DateTime now = rtc.now();
  
  if( now.hour() >= 7 && now.hour() <= 19){

    int lu = analogRead(pinOut);
    
    float voltage = 5000*(lu/1024.0) + 15;
    
    float y = offset-voltage; //en mV
    
    float value = y/sensibilite;
  
    return value;
  
  }

  return 0.0;
  
}


/**
 * Récpeurer la date et l'heure
 */
String getDate(){
   DateTime now = rtc.now();
   String date ="";
   date+=now.day();
   date+='-';
   date+=String(now.month(), DEC);
   date+='-';
   date+=String(now.year(), DEC);
   date+=" ";
   date+=String(now.hour(), DEC);
   date+=':';
   date+=String(now.minute(), DEC);
   date+=':';
   date+=String(now.second(), DEC);
   //date+='-';
   return date;
   
}


/**
 * sauvegarde des données dans la carte SD
 */
void sauvegarde(String mesure){
  DateTime now = rtc.now();
  String nomFichier = "";
  nomFichier = String(now.month(), DEC)+"_"+String(now.year(), DEC)+".txt";
  //Serial.println(nomFichier);
  String donnees = mesure+" "+getDate();

   File sauvegard = SD.open(nomFichier, FILE_WRITE);
    if(sauvegard){
      sauvegard.print(donnees);
      sauvegard.close();
      Serial.println(donnees);
      
    }else{
      Serial.println("erreur douverture du fichier");
    }
}

/**
 * Allumage de la lampe entre 19h et 7h 
 */
void allumeLampe(){
  DateTime now = rtc.now();
  if(now.hour() >= 19 || now.hour() <= 7){
    digitalWrite(LAMPE, HIGH);
    Serial.println("Lampe allumée");
  }else{
    digitalWrite(LAMPE, LOW);
  }
}

