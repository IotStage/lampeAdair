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

void setup()
{
  Serial.begin(9600);
  if (!SD.begin(4)) {
    Serial.println("echec!");
    return;
  }
  Serial.println("terminee.");
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  Wire.begin(); // Rejoindre le bus I2C (Pas besoin d adresse pour le maitre)
  delay(100);
}

void loop()
{
  String res = String(getSensorValue(), DEC);
  sauvegarde(res);
  
  //contenu du programme
  
  char buff[res.length()];
  res.toCharArray(buff, res.length());
  Wire.beginTransmission(4); // Envoyer vers device #4
  Wire.write(buff); // Envoi un 1
  Wire.endTransmission();    // Arreter la transmission
  Serial.println("donnees envoyees au slave");
  delay(2000); // Attendre 2s
  

  allumeLampe();
}


/**
 * 
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
 * Allumage de la lampe
 */
void allumeLampe(){
  DateTime now = rtc.now();
  if(String(now.hour(), DEC) >= "19" || String(now.hour(), DEC) <= "7"){ 
    digitalWrite(LAMPE, HIGH);
  }else{
    digitalWrite(LAMPE, LOW);
  }
}

