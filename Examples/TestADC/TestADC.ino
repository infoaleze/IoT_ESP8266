/*
 * Ce petit programme test la détection de 4 boutons connectés en diviseur
 * de tension à l'entrée analogique de l'ESP8266.
 * L'idée est de mettre en série 4 boutons, avec une resistance en parallèle.
 * lorsque un bouton est appuyé (actif) il court-circuite la resistance donc
 * change la valeur de la resitance global du pont diviseur. En choisissant
 * judicieusement les résistances il est possible de déterminer quels sont
 * les boutons activé ou pas. L'idée la plus simple est de choisir des 
 * resistances qui suive les puissance de 2 ( 1, 2, 4, 8...) puis ensuite
 * de l'adapter à celle qui sont disponnible dans le commerce. 
 * Dans l'expérience ci-dessous nous avons choisi : 1K, 2,2K, 3.9K et 8.2K
 * en 1%. Ensuite nous avons ajouter une resistance de rappel au +3V3 de 15K.
 * A noter que l'ADC peut mesurer au maximum 1,7V, donc aucun bouton n'est 
 * pressé nous auront une tension de 
          U = Re / Re + 15k  (ou Re est la resistance équivalente des boutons)
 * soit   U = 15250/(15000+15205)=1.66 V
 */
#include <ESP8266WiFi.h>

struct Buttons {
  bool b1;
  bool b2;
  bool b3;
  bool b4;
};


struct Buttons MesBoutons = { 0,0,0,0 };


void setup() {
    
   /* Ouverture du port USB pour le débugage */
  Serial.begin(115200);
  delay(10);

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

  
  delay(500);

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

