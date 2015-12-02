/*
 * Ce petit programme reprend le concept du TestADC pour
 * détecter quel interrupteurs sont activé via un pont diviseur
 * de tension et l'ADC interne de l'ESP8266.
 * Dans ce programme nous ajoutons un coté ludique en changeant
 * l'affichage de notre cercle de 16 Led intelligeantes.
 */
#include <ESP8266WiFi.h>
/* 
 *  Cette librairie doit être télechargée et installée dans
 *  le répertoire des librairies d'Arduino.
 *  /!\ Attention /!\ à bien prendre la librairie pour ESP8266 
 *  et notament la branch "UartDriven" :
 *  https://github.com/Makuna/NeoPixelBus/tree/UartDriven
 */
#include <NeoPixelBus.h>

// Définition de la structure qui memorise l'état des boutons
struct Buttons {
  bool b1;
  bool b2;
  bool b3;
  bool b4;
};

const int NB_PIXEL = 16; // Nombre de LED dans l'anneau
const int colorSaturation  = 192; // Saturation de la couleur


// Variable global qui stocke l'état des boutons
struct Buttons MesBoutons = { 0,0,0,0 };

// Initialisation du pilote NeoPixel
NeoPixelBus strip = NeoPixelBus(NB_PIXEL, 4, NEO_GRB);


// Création de quelques couleurs
RgbColor rgbRED   = RgbColor(colorSaturation, 0, 0);
RgbColor rgbGREEN = RgbColor(0, colorSaturation, 0);
RgbColor rgbBLUE  = RgbColor(0, 0, colorSaturation);
RgbColor rgbYELLOW = RgbColor(colorSaturation, colorSaturation, 0);
RgbColor rgbOFF   = RgbColor(0, 0, 0);

void setup() {
    
   /* Ouverture du port USB pour le débugage */
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.println("******************************************");
  Serial.println("** Petit programme de Test de l'ADC     **");
  Serial.println("** pour detecter l'activité de 4 inters **");
  Serial.println("** et afficher le resultat sur un       **");
  Serial.println("** anneau de 16 LED RGB                 **");
  Serial.println("**--------------------------------------**");
  Serial.println("** Yoann Darche  | 27/10/2015 | 1.00    **");
  Serial.println("******************************************");
  Serial.println();

  // Initialisation du Bus de Pixel
  Serial.println("[i] Initialisation du Bus de Pixel");
  strip.Begin();
  strip.Show();

}

/**************************************************************************
 * Boucle infinie                                                         *
 **************************************************************************/
void loop() {
  

  int a;

  /* Lecture de l'entrée analogique */
  a = analogRead(A0);
  Serial.println(a);
  
  /* Décodage */
  MesBoutons = decodeButton(a);

  /* Affichage */
  if(MesBoutons.b1 == 1) Serial.print("B1 "); else Serial.print("-- ");
  if(MesBoutons.b2 == 1) Serial.print("B2 "); else Serial.print("-- ");
  if(MesBoutons.b3 == 1) Serial.print("B3 "); else Serial.print("-- ");
  if(MesBoutons.b4 == 1) Serial.print("B4 "); else Serial.print("-- ");

  Serial.println("");
  setColor(rgbOFF);
  
  delay(100);

}

/*****************************************************************************
 * Fonction qui assure le décodage de l'etat des bouton en fonction de la    *
 * tension mesurée au niveau de l'ADC.                                       *
 * Les valeurs déterminé ici peuvent varier en fonction des resistances      * 
 * utilisées.                                                                *
 *        (12k+3k)   (8.2k)  (3.9k)  (2.2k)   (1k) 1%                        *
 * 3v3 -- 14,970k -+- 8180 -- 3880 -- 2198 --  992 -- 0v                     *                                                     
 *                 |  \B4/    \B3/    \B2/    \B1/                           *
 *                ADC                                                        *
 *****************************************************************************/
struct Buttons decodeButton(int a) {

  struct Buttons res = { 0, 0, 0, 0 };

  if (a >= 555) return res;

  if (a >= 540) { res.b1 = 1; return res; }
  if (a >= 518) { res.b2 = 1; return res; }
  if (a >= 497) { res.b1 = 1; res.b2 = 1; return res; }
  if (a >= 483) { res.b3 = 1; return res; }
  if (a >= 450) { res.b1 = 1;  res.b3 = 1; return res; }
  if (a >= 428) { res.b2 = 1; res.b3 = 1; return res; }
  if (a >= 400) { res.b1 = 1; res.b2 = 1; res.b3 = 1; return res; }
  if (a >= 360) { res.b4 = 1; return res; }
  if (a >= 320) { res.b1 = 1; res.b4 = 1; return res; }
  if (a >= 280) { res.b2 = 1; res.b4 = 1; return res; }
  if (a >= 237) { res.b1 = 1; res.b2 = 1; res.b4 = 1; return res; }
  if (a >= 200) { res.b3 = 1; res.b4 = 1; return res; }
  if (a >= 152) {  res.b1 = 1; res.b3 = 1; res.b4 = 1; return res; }
  if (a >= 77) {  res.b2 = 1; res.b3 = 1; res.b4 = 1; return res; }

  res.b1 = 1; 
  res.b2 = 1;  
  res.b3 = 1; 
  res.b4 = 1; 
  
  return res; 
}


/***************************************************************
 * Foncion qui met à jour toutes les LED.                      *
 * Les quatre premiere LED piloter par le bouton 1, les 4      *
 * suivante par le bouton 2 etc..                              *
 ***************************************************************/
void setColor(RgbColor color) {
  Serial.println("[i] Envoie des donnees au LED");
  int i, B;
  for (i=0; i < NB_PIXEL; i++) {

    B = i % 4;

    if ((B==0) && (MesBoutons.b1 == 1)) 
      strip.SetPixelColor(i, rgbRED);
    else if  ((B==1) && (MesBoutons.b2 == 1)) 
      strip.SetPixelColor(i, rgbGREEN);
    else if  ((B==2) && (MesBoutons.b3 == 1))
      strip.SetPixelColor(i, rgbBLUE);   
    else if  ((B==3) && (MesBoutons.b4 == 1)) 
      strip.SetPixelColor(i, rgbYELLOW);   
    else
    {
      //Serial.print(">>"); Serial.print(B); Serial.print(" - "); Serial.print(i);
      //Serial.println(" : Default");
      strip.SetPixelColor(i, color);        
    }
  }
  strip.Show();
}

