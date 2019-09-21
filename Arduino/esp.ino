#include <ESP8266WiFi.h>
 
#define HLPIN A0 // pin de datos del HL-69

String apiKey = "T5MRA66PHEPY0EHX"; // poner aquí su respectiva api key de thingspeak

const char *ssid =  ""; // nombre de la red wifi
const char *pass =  ""; // contraseña de la red wifi
const char* server = "api.thingspeak.com"; // el enlace hacia donde se van a mandar los datos

WiFiClient client;
 
void setup() {
       Serial.begin(115200);
 
       Serial.print("Conectandose a: ");
       Serial.print(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) { // no va a continua hasta que se conecte a la red wifi
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("────────────CONECTADO────────────");
      randomSeed(4352); // da un origen para random
}
 
void loop(){
    int randNumber = random(10, 40); // genera un numero aleatoreo entre 10 y 39
    float hl = analogRead(HLPIN); // obtiene la información del hl-69
    if (client.connect(server,80))   // Aqui inicia la conexión con thingspeak{          
        String postStr = apiKey; // esta variable almacena los datos que vamos a mandar
        postStr +="&field1=";
        postStr += String(hl);
        postStr +="&field2=";
        postStr += String(randNumber);
        postStr += "\r\n\r\n";

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);
        client.stop();
        Serial.println("Waiting...");
        // thingspeak necesita minimo 15 segundos para poder mandarle más datos
        delay(15000); // la función delay maneja el tiempo en milisegundos
    }
    else{ // esto es en caso de que no se conecte con thingspeak
        Serial.println("No se pudo conectar con thingspeak");
        delay(1000);
    }
}