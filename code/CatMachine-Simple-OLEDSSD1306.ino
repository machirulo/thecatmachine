/*
 * T H E C A T M A C H I N E
 * POWERFULL MODERNISEISHON IOT DEVICE SHOWING USEFUL AND USELESS DATA
 * BY MACHIRULO
 */ 

/*IMPORTANTE:
 * El codigo es un asco, pero anda. ni ganas de optimizar nada. as is. El que quiera optimizar, feel free.
 * 
 *  ESTA COSA CORRE EN UN ESP8266. IDEALMENTE UN NODEMCU 1.0.
 *  
 *  NECESITAN LAS LIBRERIAS ssd1306 PARA EL OLED, ARDUINOJSON, Y LAS DEL ESP8266 PARA EL IDE DE ARDUINO
 *  
 * EL CODIGO SE ENTREGA COMO ES- EL AUTOR NO SE RESPONSABILIZA POR SU MAL USO, NI POR LA VERACIDAD O EXACTITUD DE LOS DATOS DE TERCEROS.
 * EL MISMO POR SI SOLO PRETENDE MOSTRAR LAS FUNCIONALIDADES QUE TIENE EL ESP8266 PARA PROCESAR JSON DE DIFERENTES APIs.
 * */

#include <ESP8266WiFi.h>
#include <Wire.h>  
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

#include <ArduinoJson.h>


#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   


boolean primera;



const char* host = "ws.geeklab.com.ar";
String line;
SSD1306  display(0x3c, D3, D5); 


void setup() {
   mySoftwareSerial.begin(9600);
   randomSeed(10);
  Serial.begin(115200);
  delay(10);

  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
display.clear(),
display.setTextAlignment(TEXT_ALIGN_LEFT);
display.drawString(0,0, "THE CAT");
display.drawString(0,30, "MACHINE");
display.display();

WiFiManager wifiManager;

// wifiManager.resetSettings(); //dependiente de un pulsador.
  wifiManager.autoConnect("AutoConnectAP");
 
}

void loop() {

  
  delay(60000);

  
  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 80;
  display.clear(),
display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);

display.drawString(0,0, "CONECTANDO");
display.drawString(0,30, "AL MUNDO");
display.display();
  if (!client.connect(host, httpPort)) {

    
    Serial.println("connection failed");
    return;
  }

  


  
  String url = "/dolar/get-dolar-json.php";
 
  Serial.print("Requesting URL: ");
  Serial.println(url);
  

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  

  while(client.available()){
    line = client.readStringUntil('\r');
    Serial.print(line);
  }
   DynamicJsonBuffer jsonBuffer;
   JsonObject& root = jsonBuffer.parseObject(line);
   //float oficial= root[String("libre")];
//float blue= root[String("blue")];
String oficial = root["libre"];
String blue = root["blue"];

display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 0, String("libre:"+oficial));
    display.drawString(0, 30, String("blue"+blue));
    display.display();
  Serial.println();
  Serial.println("closing connection");
}



