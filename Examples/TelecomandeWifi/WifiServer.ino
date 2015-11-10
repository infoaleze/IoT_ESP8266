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

const char* ssid = "<SSID - Achanger>";
const char* password = "<Pwd - Achanger>";

// Création d'un server sur le port 80 (généralement utilisé par le WEB)
WiFiServer server(80);

/* *********************************************************************** *
 *  Setup : initialisation des services, mode des entrées/sorties etc...   *
 * *********************************************************************** */
void setup() {

  int Led0 = LOW;
  
  /* Ouverture du port USB pour le débugage */
  Serial.begin(115200);
  delay(10);

  /* Dans cette exemple nous utiliserons 
    - le port D0 soit GPIO16 pour la led intégrée bleu
    - le port D1 soit GPIO5  pour la led (en mode OUTPUT)    
  */
  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  digitalWrite(16, HIGH);

  Serial.println();
  Serial.println();
  Serial.println("**************************************");
  Serial.println("** Petit programme de Test Wifi     **");
  Serial.println("** Permet de piloter une LEd via    **");
  Serial.println("** une URL ( http://<IP>/gpio/[0|1] **");
  Serial.println("**----------------------------------**");
  Serial.println("** Yoann Darche | 21/10/2015 | 1.00 **");
  Serial.println("**************************************");
  Serial.println();
    
  // initialisation de la connexion au réseau Wifi  
  Serial.print("[i] Tentative de connexion a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  // On attend que la connexion soit opérationelle
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(Led0 == LOW) { Led0 = HIGH; } else { Led0 = LOW; }
    digitalWrite(16, Led0);
  }
  Serial.println("");
  Serial.println("[+] WiFi connecte");
  
  // Démarre le serveur
  server.begin();
  Serial.println("[+] Serveur demarree");

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
  
  // Affiche dans la console de débugage l'adresse IP 
  // utilisée
  Serial.print("L'adresse IP utilise est :");
  Serial.println(WiFi.localIP());
}

/* *********************************************************************** *
 *  Loop : Programme principal                                             *
 * *********************************************************************** */
void loop() {
  // Vérifie si un client est connécté au module
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Attend que le client (browser) envoie des données
  // dans notre cas la requête HTTP
  Serial.println("[i] Nouveau client");
  while(!client.available()){
    delay(1);
  }
  
  // Lecture de la première ligne envoyé par le client
  // dans notre cas GET <url>
  String req = client.readStringUntil('\r');
  Serial.println(req);
  // On vide le reste des données reçus (économie de mémoire)
  client.flush();
  
  // Décodage de la requête reçue
  int val;
  if (req.indexOf("/gpio/0") != -1) {
    val = 0;
    // Désactivation du GPIO5 soit D1
    digitalWrite(5, val);
  }
  else if (req.indexOf("/gpio/1") != -1) {
    val = 1;
    // Activation du GPIO5 soit D1
    digitalWrite(5, val);
  }
  else {
    Serial.println("[-] Requete invalide");
  }
  
  client.flush();

  // Entête de Réponse   
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  // Ajout de la référence à une stylesheet pour faire des beaux boutons sans charger la mémoire
  s+= "<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"http://infoaleze.chez.com/IoT/IoT.css\" media=\"all\"/></head><body>";
  // Ecriture de l'état du port
  s+="<h1>D1(GPIO5) est maintenant ";
  s += (val)?"On":"Off";
  s += "</h1></html>\n";
  // Ajout de bouton de commutation
  s += "<a href=\"../gpio/1\" class=\"bouton\">On</a><a href=\"../gpio/0\" class=\"bouton\">Off</a></body></html>";
  
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("[i] Client deconecte");

  // Quand nous sortons de la fonction, la variable objet client sera automatiquement détruite
  // et donc la liaison avec le client coupée
}
