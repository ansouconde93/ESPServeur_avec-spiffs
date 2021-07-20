#include <Arduino.h>
#include <../.pio/libdeps/esp32dev/ESP Async WebServer/src/ESPAsyncWebServer.h>
#include <SPIFFS.h>
const char* ssid = "your connexion ssid here";
const char* password ="your connexion password here";
AsyncWebServer server(80);

const int pwmPin = 16;
const int freq = 500;
const int ledChannel = 0;
const int resolution = 8;


void setup() {
  Serial.begin(115200);
  while (!Serial){}
  Serial.println("\n");

  if(! SPIFFS.begin()){
    Serial.println("Erreur de demarrage du SPIFFS !");
    return;
  }
 
  //----------------------------------------------------WIFI
  WiFi.begin(ssid, password);
	Serial.print("Tentative de connexion...");
	
	while(WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(100);
	}
	
	Serial.println("\n");
	Serial.println("Connexion etablie!");
	Serial.print("Adresse IP: ");
	Serial.println(WiFi.localIP());

ledcSetup(ledChannel,freq,resolution);
ledcAttachPin(pwmPin,ledChannel);
  //----------------------------------------------------SERVER
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  
  server.on("/gb1.jpg", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/gb1.jpg", "image/jpg");
  });
  server.on("/interaction.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/interaction.js", "text/javascript");
  });
  server.on("/jquery.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/jquery.js", "text/javascript");
  });
  server.on("/bncache.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/bncache.js", "text/javascript");
  });

  server.on("/allumer", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    ledcWrite(ledChannel, 255);
    request->send(200);
  });

  server.on("/eteindre", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    ledcWrite(ledChannel, 0);
    request->send(200);
  });
  server.on("/nuit", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    for(int cycle =0; cycle <=200; cycle++){
      ledcWrite(ledChannel, cycle);
      delay(15);
    }
    request->send(200);
  });

  server.on("/jour", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    for(int cycle =220; cycle >=50; cycle--){
      ledcWrite(ledChannel, cycle);
      delay(15);
    }
    request->send(200);
  });

  server.begin();
  Serial.println("Serveur actif!"); 
  
}

void loop() {
}
