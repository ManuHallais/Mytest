
#include <Adafruit_GFX.h>    // Biliothèque graphique 
#include <Adafruit_ST7735.h> // Biliothèque du contrôleur de l'écran
#include <SPI.h>             // Biliothèque de gestion de la communication SPI
#include "ESP8266WiFi.h"     //Biliothèque de l'ESP
 

      // Déclaration des broches nécessaires à connecter
#define TFT_CS     D4
#define TFT_RST    -1  
#define TFT_DC     D3

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


      //Déclaration des données du réseau
const char* ssid = "Bird House";
const char* password =  "TWEET420";

      // Déclaration du serveur et du port de communication
WiFiServer wifiServer(8888);


String Temp;
String Hum;
                                // Initialisation de la carte
void setup() {
  Serial.begin(115200); //Initialisation de la vitesse de communication sur le port série pour debug
  delay(1000);
  
                                //Connexion au réseau
  WiFi.begin(ssid, password);  
   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());   //Affichage de l'adresse IP
 
  wifiServer.begin();   //Lancement du serveur


                                //Initialisation de l'écran

  // Use this initializer if you're using a 1.8" TFT
  //tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  // Use this initializer (uncomment) if you're using a 1.44" TFT
  tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab

  Serial.println("Initialized");
  
  tft.setRotation(1);   //Configuration du sens de lecture de l'écran
  tft.fillScreen(ST7735_BLACK);   //Remplissage en noir de l'écran (effacement)
  
  
  Serial.println("done");
  delay(1000);
}

                                //Boucle principale
void loop() {
                          //Déclaration des variables locales
String k;
String m;
char c;
int f;
        
   WiFiClient client = wifiServer.available();
 
  if (client) {   
    while (client.connected()) {         
      while (client.available()>0) {     
        c = client.read();
        Serial.write(c);
        Serial.print("gsdfg: ");       
        Serial.println(c);
        k+=c;           
      }
        
      
        
      if (client.read()<0){
        int longr=k.length();
        m=k.substring(0,longr-1);
        f=0;
        k="";
        SepareValeurs(m);                                    
      }
        
      else {
        f=1;
        
      }
      
      if (f==0 && k==""){ 
        tft.fillScreen(ST7735_BLACK);
      }
      
      
      
      delay(10);
                              //Affichage des données sur l'écran    
      tft.setCursor(40, 10);
      tft.setTextColor(ST7735_YELLOW);
      tft.setTextSize(2);
      tft.setTextWrap(true);
  
      tft.println("Temp: ");
      tft.setCursor(35, 35);
      tft.print(Temp);
      tft.print(" ");
      tft.print( (char)247);
      tft.println("C");
  
      tft.setCursor(45, 60);
      tft.println("Hum:");
      tft.setCursor(10, 85);
 
      tft.print(Hum);
      tft.println(" %HR");
      delay(5000);    
    }        
  }
                        //Déconnexion du client
      client.stop();
      Serial.println("Client disconnected");
}

void SepareValeurs(String str){
  int commaIndex = str.indexOf(',');
  //  Search for the next comma just after the first
  //int secondCommaIndex = myString.indexOf(',', commaIndex + 1);

  Temp = str.substring(0, commaIndex);
  Hum = str.substring(commaIndex + 1);
}
