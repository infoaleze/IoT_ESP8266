/*
 *  Ce petit programme démontre la réalisation d'un tout petit serveur
 *  HTTP, pour interagir avec le module ESP via le Wifi.
 *  Dans cette exemple nous utilisons un réseau Wifi existant, il faudra
 *  donc fournir les paramètres ssid & password en fonction de votre
 *  situation.
 *  
 *  Afin de pouvoir commuter la sortie D1 de nodeMCU, il est nécessaire
 *  d'appeler l'adresse IP du module suivit de /gpio/1 pour activer ou
 *  /gpio/0 pour désactiver
 *  
 *  Exemple : http://<IP>/gpio/0
 *  
 *  Les informations concernant l'état du module sont envoyées sur la
 *  ligne série (115200 bauds)
 *  
 *  Le clignotement lent de la LED indique une tentative de connexion
 *  à votre réseau Wifi
 *  Le clignotement rapide indique que la connexion est établie (le 
 *  clignotement s'arrête en suite)
 *  
 *  La page Web renvoyée à l'utilisateur est simple, mais elle utilise
 *  un CSS qui est stocké sur http://infoaleze.chez.com/IoT afin 
 *  d'améliorer le visuel
 */

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <NeoPixelBus.h>
#include <Streaming.h>



const char* ssid = "INFOALEZE";
const char* password = "INFOALEZE";

const int NB_PIXEL = 16; // Nombre de LED dans l'annau
const int colorSaturation  = 192; // Saturation de la couleur

IPAddress UDP_GROUP(239, 192, 0, 100);
unsigned int UPD_PORT = 4242;


// Création d'une instance UPD pour envoyer et recevoir des msg UPD
WiFiUDP Udp;

// Initialisation du pilote NeoPixel
NeoPixelBus strip = NeoPixelBus(NB_PIXEL, 4, NEO_GRB);


RgbColor rgbRED = RgbColor(colorSaturation, 0, 0);
RgbColor rgbGREEN = RgbColor(0, colorSaturation, 0);
RgbColor rgbBLUE = RgbColor(0, 0, colorSaturation);
RgbColor rgbWHITE = RgbColor(255, 255, 255);
RgbColor rgbOFF = RgbColor(0, 0, 0);


/* *********************************************************************** *
 *  Setup : initialisation des services, mode des entrées/sorties etc...   *
 * *********************************************************************** */
void setup() {

  int Led0 = LOW;
  int cpt = 0;
  
  /* Ouverture du port USB pour le débugage */
  Serial.begin(115200);
  delay(10);

  /* Dans cette exemple nous utiliserons 
    - le port D0 soit GPIO16 pour la led intégrée bleu
    - le port D1 soit GPIO5  pour la led (en mode OUTPUT)    
    - le port D2 soit GPIO4  pour l'anneau de LED WS2812 - VIA UART1
  */
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  
  digitalWrite(5, LOW);
  digitalWrite(16, HIGH);
  

  Serial.println();
  Serial.println();
  Serial.println("***************************************");
  Serial.println("** Petit programme de Test Wifi      **");
  Serial.println("** Permet de piloter un anneau de 16 **");
  Serial.println("** Leds (WS2812) via Protocole UDP   **");
  Serial.println("**-----------------------------------**");
  Serial.println("** Yoann Darche  | 04/11/2015 | 1.00 **");
  Serial.println("***************************************");
  Serial.println();


  // Initialisation du Bus de Pixel
  Serial.println("[i] Initialisation du Bus de Pixel");
  strip.Begin();
  strip.Show();
    
  // initialisation de la connexion au réseau Wifi  
  Serial.print("[i] Tentative de connexion a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);  

  // On attend que la connexion soit opérationelle
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");    
    setIncremental(rgbBLUE, cpt);
    cpt = cpt + 1;
    if(Led0 == LOW) { Led0 = HIGH; } else { Led0 = LOW; }
    digitalWrite(16, Led0);
  }
  Serial.println("");
  Serial.println("[+] WiFi connecte");
  
  /* Les lignes suivantes réalisent une petite anim
   *  sur la LED intégrée afin d'avertir que le
   *  module est prêt et connecté
   */
  digitalWrite(16, LOW);
  delay(392);
  digitalWrite(16, HIGH);
  delay(392);
  for(int i=0; i<10; i++) {
    digitalWrite(16, LOW);
    delay(125);
    digitalWrite(16, HIGH);
    delay(125);
  }
  setColor(rgbOFF);

  // Démarage du serveur UDP (pour capter les message
  if(Udp.beginMulticast(WiFi.localIP(),UDP_GROUP,UPD_PORT) == 0) {
    Serial << "[-] Erreur lors du demarrage du serveur UDP/Multicast" << endl;
  }
  else {
     Serial << "[+] Serveur UPD/Multicast demarre sur le groupe:" << UDP_GROUP << ", port:" << UPD_PORT << endl;
  }
  
  // Affiche dans la console de débugage l'adresse IP 
  // utilisée
  Serial.print("L'adresse IP utilise est :");
  Serial.println(WiFi.localIP());
}

/* *********************************************************************** *
 *  Loop : Programme principal                                             *
 * *********************************************************************** */
void loop() {
    //MUDPSayHello();
    delay(100);
    if(Udp.available() >= 1){
       Serial << "Passer le test" << endl;
       MUDPReadWifi();
       Udp.flush();
    }
  
}


/***************************************************************
 * Fonction pour recevoir un caractere       *
 ***************************************************************/
void MUDPReadWifi() {
char CharIn; 
          
   /* Udp.beginPacket(UDP_GROUP, UPD_PORT); */

   if(Udp.parsePacket() >= 1)
   {
    //CharIn = Udp.read(WiFi.localIP());
   Serial << "parsePacket() ok" << endl;
   
/* Udp.print(WiFi.localIP()); */
//   Udp.endPacket();

 //  CharIn = getch();
    CharIn =  Udp.peek();
    Serial << "Caractere recu : " << CharIn << endl;
    
    switch (CharIn){
      case 'R':
        setColor(rgbOFF);
        Serial.print("Reset effectue.");
        break;
      case 'P':
        setColor(rgbGREEN);
        break;
      case 'A':
        setColor(rgbRED);
        break;
      default:
        return ;
        break;      
      }
     }
   Serial.println("[i] MUDPSayHello : packet multicast envoye");
}

/***************************************************************
 * Fonction pour envoyé un Hello sur le groupe MULTICAST       *
 ***************************************************************/
void MUDPSayHello() {
  
   Udp.beginPacket(UDP_GROUP, UPD_PORT);
   Udp.write("Salut! :");
   Udp.print(WiFi.localIP());
   Udp.endPacket();
   Serial.println("[i] MUDPSayHello : packet multicast envoye");
}


/***************************************************************
 * Fonction qui met à jour toutes les LED.                     *
 ***************************************************************/
void setColor(RgbColor color) {
  Serial.println("[i] Envoie des donnees au LED");
  int i;
  for (i=0; i < NB_PIXEL; i++) {
    strip.SetPixelColor(i, color);
  }
  strip.Show();
}

/*************************************************************
 * Fonction qui affiche un avancement sur l'anneau           *
 *************************************************************/
void setIncremental(RgbColor color, int nb) {
  
  int k = nb % NB_PIXEL;
  int i;
  float z;

  //Serial.println("setIncremental : in");
  
  for(i=0; i < NB_PIXEL; i++) {
    if(i <= k) { 


       z = (k==0)?1.0:(i*1.0)/k;
       
      //Serial << ">> nb = " << nb << ", k =" << k << ",i =" << i << ", z=" << z << endl;      
      
      strip.SetPixelColor(i, RgbColor::LinearBlend(rgbRED, color, z)); 
    }
    
    else
      strip.SetPixelColor(i, rgbOFF);     
  }
  strip.Show();
}
 
