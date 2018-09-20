/*
 * T H E C A T M A C H I N E
 * POWERFULL MODERNISEISHON IOT DEVICE SHOWING USEFUL AND USELESS DATA
 * BY MACHIRULO
 */ 

/*IMPORTANTE:
 * El codigo es un asco, pero anda. ni ganas de optimizar nada. as is. El que quiera optimizar, feel free.
 * Para usar el dispositivo,tienen que crear una cuenta en GEONAMES.org y colocar su nombre de usuario en las APIs usadas.
 * 
 * http://api.geonames.org/weatherIcaoJSON?ICAO=SABE&username=aca_tu_nombre_usuario
 *  ESTA COSA CORRE EN UN ESP8266. IDEALMENTE UN NODEMCU 1.0.
 *  
 *  NECESITAN LAS LIBRERIAS LIQUIDCRYSTAL_I2C, ARDUINOJSON, Y LAS DEL ESP8266 PARA EL IDE DE ARDUINO
 *  
 * EL CODIGO SE ENTREGA COMO ES- EL AUTOR NO SE RESPONSABILIZA POR SU MAL USO, NI POR LA VERACIDAD O EXACTITUD DE LOS DATOS DE TERCEROS.
 * EL MISMO POR SI SOLO PRETENDE MOSTRAR LAS FUNCIONALIDADES QUE TIENE EL ESP8266 PARA PROCESAR JSON DE DIFERENTES APIs.
 * */
 

#include <ESP8266WiFi.h>
#include <Wire.h>  

#include <LiquidCrystal_I2C.h>

//prestar atencion a donde va el i2c en su placa basada en elESP8266. En la Nodemcu de Lolin son D1 y D2.
LiquidCrystal_I2C lcd(0x27, 16, 2); 

#include <ArduinoJson.h>


#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   



const char* host_dolar = "ws.geeklab.com.ar";
const char* parametros_dolar = "/dolar/get-dolar-json.php";
const char* host_clima = "api.geonames.org";

//reemplazar por elnombre de usuario:
const char* parametros_clima= "/weatherIcaoJSON?ICAO=SABE&username=nombre_usuario"; 
////////////////////////////////

const char* host_eco = "apis.datos.gob.ar";
const char* parametros_ipc = "/series/api/series/?ids=102.1_I2NG_ABRI_M_22:value&format=json&metadata=none&sort=desc&limit=1&start_date=2018";
const char* parametros_inflacion= "/series/api/series/?ids=431.1_EXPECTATIVDIO_M_0_0_30_56:value&format=json&metadata=none&sort=desc&limit=1&start_date=2018";
const char* parametros_canasta_dif = "/series/api/series/?ids=150.1_CSTA_BATAL_0_D_20&metadata=none&representation_mode=percent_change_a_year_ago&sort=desc&limit=1&start_date=2018";

const char* parametros_canasta_pesos= "https://apis.datos.gob.ar/series/api/series/?ids=150.1_CSTA_BATAL_0_D_20&metadata=none&representation_mode=value&sort=desc&limit=1&start_date=2018";


String line;

unsigned long timeout; //milis para contar la freq de refresco.

unsigned long prev_timeout_clima;
unsigned long prev_timeout_dolar;
unsigned long prev_timeout_hora;

unsigned long prev_timeout_lcd; 

//refresco de datos:
unsigned long freq_clima= 900000; //15minutos
unsigned long freq_dolar= 900000;
unsigned long freq_hora= 60000; //1 minuto
/////////////////


int freq_refresco_lcd= 5000;  //funcion de refresco de pantalla (no de datos)
boolean primer_inicio=true;

const char* host_hora = "api.geonames.org";
 //REEMPLAZAR POR NOMBRE DE USUARIO:
const char* parametros_hora= "/timezoneJSON?lat=-34.35&lng=-58.22&username=nombre_usuario"; 
/////////////////

//datos geonames
int temp;
int humedad;
int viento_dir;
int hPa;
const char* viento_vel;
const char* hora;

//datos apidolar
const char* oficial;
const char* blue;

//Datos de datos.gob.ar
const char* ipc;
const char* inflacion;
const char* canasta_pesos;
float canasta_dif;

const char* ultima_fecha_ipc;
const char* ultima_fecha_inflacion;
const char* ultima_fecha_canasta;

DynamicJsonBuffer jsonBuffer(700);


void setup() {
  pinMode(D3,OUTPUT); //PIN LEDs ojos. Opcional si se quieren poner leds que parpadeen al actualizarse el dolar.
  pinMode(D4,OUTPUT); // Esta salida es opcion, es para comandar un modulo reproductor de sonido ISD1820 (pin PE del ISD). Solo al actualizar el dolar.
  digitalWrite(D3,LOW);
  digitalWrite(D4,LOW);
  lcd.begin();
lcd.clear();
lcd.home();
  lcd.setCursor(1,0);
  lcd.print("THE CAT MACHINE");
  lcd.setCursor(0, 1);
  lcd.print("by Machirulo");

digitalWrite(D4,HIGH);
delay(10);
digitalWrite(D4,LOW);
for (int i = 0; i<10; i++)
{digitalWrite(D3,LOW);
  delay(100);
  digitalWrite(D3,HIGH);
  delay(100);
  }

  Serial.begin(115200);

 
WiFiManager wifiManager;
 
// wifiManager.resetSettings(); //dependiente de un pulsador...paja
  wifiManager.autoConnect("CatMachine-elija-wifi");

}

void loop() {

  while (WiFi.status() != WL_CONNECTED) {
     lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CONECTANDO...");
  lcd.setCursor(0, 1);
  lcd.print("AL MUNDOO!");
  }


if (primer_inicio){  //Aca se recogen rapidamente los datos para mostrarlos alprincipio, despues se actualiza cada uno a su frecuencia.

 get_dolar(host_dolar,parametros_dolar,0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("u$s ofi.:$");
  lcd.print(oficial);
  lcd.setCursor(0, 1);
  lcd.print("u$s blue:$");
  lcd.print(blue);
  for (int i = 0; i<10; i++)
{digitalWrite(D3,LOW);
  delay(100);
  digitalWrite(D3,HIGH);
  delay(100);
  }
  digitalWrite(D4,HIGH);
delay(10);
digitalWrite(D4,LOW);
 delay(5000);
 
 ///////////////////////////// INFLACION
 get_inflacion(host_eco, parametros_inflacion);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Inflacion Exp.");
  lcd.setCursor(0, 1);
  lcd.print(ultima_fecha_inflacion);
  lcd.setCursor(11, 1);
  lcd.print(inflacion);
  lcd.setCursor(15, 1);
  lcd.print("%");

delay(5000);


 ////////////////////////////////////IPC
get_ipc(host_eco, parametros_ipc),
 lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("IPC Mensual");
  lcd.setCursor(0, 1);
  lcd.print(ultima_fecha_ipc);
  lcd.setCursor(11, 1);
  lcd.print(ipc);
  delay(5000);



 ////////////////////////////////////CANASTA BASICA DIF
 get_canasta_dif(host_eco, parametros_canasta_dif);
 lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Can.Bas-dif-1ano");
  lcd.setCursor(0, 1);
  lcd.print(ultima_fecha_canasta);
  lcd.setCursor(11, 1);
  lcd.print(canasta_dif);
lcd.setCursor(15, 1);
lcd.print("%");
 delay(5000);

 ///////////////////////////////////CANASTA PESOS CORRIENTES
 get_canasta_pesos(host_eco, parametros_canasta_pesos);

  lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Canasta Basica");
  lcd.setCursor(0, 1);
  lcd.print(ultima_fecha_canasta);
  lcd.setCursor(12, 1);
  lcd.print(canasta_pesos);
  lcd.setCursor(11, 1);
  lcd.print("$");
  
 delay(5000);
/////////////////////////////////////////CLIMA

 get_clima(host_clima,parametros_clima,0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:"+String(temp)+" oC"+"  "+"H:"+String(humedad)+" %");
  lcd.setCursor(5, 1);
  lcd.print("hPa: "+String(hPa));

 delay(5000);
 //////////////////////////////////////HORA
 
 get_hora(host_hora, parametros_hora,0);
    lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Fecha y Hora");
  lcd.setCursor(0, 1);
  lcd.print(hora);
  for (int i = 0; i<10; i++)
{digitalWrite(D3,LOW);
  delay(100);
  digitalWrite(D3,HIGH);
  delay(100);
  }
 delay(5000);




 primer_inicio=false;
 }
 
  lcd_display(freq_refresco_lcd); //funcion para mostrar en pantalla (el valor es la freq de actualizacion)
 get_dolar(host_dolar,parametros_dolar,freq_dolar);  //funciones de actualizacion desde las API.
  get_clima(host_clima,parametros_clima,freq_clima);
 get_hora(host_hora, parametros_hora,freq_hora);


}

///////////////Funciones para GETear todo (deberia haber sido una sola, pero cada JSON tiene sus mañas)

void get_dolar(const char* host, const char* parametros, unsigned long freq ) 
{  

 timeout = millis();
    if(timeout - prev_timeout_dolar >= freq){
    
     WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {

    return;
  }
  
  // This will send the request to the server
  client.print(String("GET ") + parametros + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    line = client.readStringUntil('\r');
   // Serial.print(line);
  }
   
   JsonObject& root = jsonBuffer.parseObject(line);
   //float oficial= root[String("libre")];
//float blue= root[String("blue")];




 oficial = root["libre"];
blue = root["blue"];

    prev_timeout_dolar=timeout;
    
    
 digitalWrite(D4,HIGH);
delay(10);
digitalWrite(D4,LOW);
  }
}


  void get_hora(const char* host, const char* parametros, unsigned long freq) 
{  
 timeout = millis();

     if(timeout - prev_timeout_hora >= freq){
         prev_timeout_hora=timeout;
    
    
  
     WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {

    return;
  }
  

  
  // This will send the request to the server
  client.print(String("GET ") + parametros + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while (client.available() == 0) {
    if (millis() - timeout > 7000) {
     // Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()>1){
    line = client.readStringUntil('\n');
    line= line.substring(line.indexOf("{"));
    line.trim();
   
  }
   
    
   
   JsonObject& root = jsonBuffer.parseObject(line);
  //root.printTo(Serial);

if(root.success())
{
hora = root ["time"];
  } 

//hora= hora.substring(11);



  }}


  
void get_clima(const char* host, const char* parametros, unsigned long freq) 
{ 
 timeout = millis();

  if(timeout - prev_timeout_clima >= freq){
         prev_timeout_clima=timeout;
 
 
 
     WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {

    return;
  }
  

  
  // This will send the request to the server
  client.print(String("GET ") + parametros + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
 
  while(client.available()){
    
    line = client.readStringUntil('\n');
    line= line.substring(line.indexOf("{"));
    //viene con porquerias,poreso.
  }
    line.trim();// Porque manda mierda \n y un cero,
    //nunca un JSON puro, putEs

  
   JsonObject& root = jsonBuffer.parseObject(line);
JsonObject& weatherObservation = root["weatherObservation"];
//root.printTo(Serial);



temp = weatherObservation["temperature"]; 
humedad = weatherObservation["humidity"];
viento_dir = weatherObservation["windDirection"]; 
hPa = weatherObservation["hectoPascAltimeter"]; 
viento_vel = weatherObservation["windSpeed"];
  




  }
  }


void get_inflacion(const char* host, const char* parametros)
{   timeout = millis();
     WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {

    return;
  }
  client.print(String("GET ") + parametros + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while (client.available() == 0) {
    if (millis() - timeout > 10000) {
      Serial.println(">>> Client Timeout Inflacion !");
      client.stop();
      return;
    }
  }
  
  while(client.available()){
    line = client.readStringUntil('\r');
    //line= line.substring(line.indexOf("{"));
  }


   JsonObject& root = jsonBuffer.parseObject(line);
if(root.success())
{
ultima_fecha_inflacion = root["data"][0][0]; 
inflacion = root["data"][0][1]; 
Serial.print("Inflacion:");  ///DEBUGGING
Serial.print(ultima_fecha_inflacion);
Serial.println(inflacion);

  } 
  }
  
void get_ipc(const char* host, const char* parametros)  
{ timeout = millis();
  WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {

    return;
  }
  client.print(String("GET ") + parametros + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while (client.available() == 0) {
    if (millis() - timeout > 7000) {
     // Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  while(client.available()){
    line = client.readStringUntil('\r');
    //line= line.substring(line.indexOf("{"));

   
  }
   
    

   JsonObject& root = jsonBuffer.parseObject(line);


if(root.success())
{
  
ultima_fecha_ipc = root["data"][0][0]; // "2018-08-01"
ipc = root["data"][0][1]; // 35.3

Serial.print("IPC:");
Serial.print(ultima_fecha_ipc);
Serial.println(ipc);

  }
  
  }


void get_canasta_dif(const char* host, const char* parametros)
{  timeout = millis();
WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {

    return;
  }
  client.print(String("GET ") + parametros + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while (client.available() == 0) {
    if (millis() - timeout > 7000) {
     // Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  while(client.available()){
    line = client.readStringUntil('\r');
    //line= line.substring(line.indexOf("{"));

   
  }
   
    
   client.stop();
   JsonObject& root = jsonBuffer.parseObject(line);


if(root.success())
{
ultima_fecha_canasta = root["data"][0][0]; // "2018-08-01"
canasta_dif = root["data"][0][1]; // 35.3
canasta_dif=canasta_dif*100;
Serial.print("Canasta pesos:");
Serial.print(ultima_fecha_canasta);
Serial.println(canasta_dif);

  } 
  
  
  }


void get_canasta_pesos(const char* host, const char* parametros)
{  timeout = millis();
WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {

    return;
  }
  client.print(String("GET ") + parametros + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  while (client.available() == 0) {
    if (millis() - timeout > 7000) {
     // Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  while(client.available()){
    line = client.readStringUntil('\r');
    //line= line.substring(line.indexOf("{"));

   
  }
   
    
   client.stop();
   JsonObject& root = jsonBuffer.parseObject(line);


if(root.success())
{

canasta_pesos = root["data"][0][1]; // 35.3

Serial.print("Canasta pesos:");
Serial.print(ultima_fecha_canasta);
Serial.println(canasta_pesos);

  } 
  
  }



/////////////Funcion de display LCD

void lcd_display(int intervalo)
  {
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T:"+String(temp)+" oC"+"  "+"H:"+String(humedad)+" %");
  lcd.setCursor(5, 1);
  lcd.print("hPa: "+String(hPa));

  
   delay(intervalo);
           
     lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Fecha y Hora");
  lcd.setCursor(0, 1);
  lcd.print(hora);
  for (int i = 0; i<10; i++)
{digitalWrite(D3,LOW);
  delay(100);
  digitalWrite(D3,HIGH);
  delay(100);
  }
 
          delay(intervalo);    
lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("u$s ofi.:$");
  lcd.print(oficial);
  lcd.setCursor(0, 1);
  lcd.print("u$s blue:$");
  lcd.print(blue);
  for (int i = 0; i<10; i++)
{digitalWrite(D3,LOW);
  delay(100);
  digitalWrite(D3,HIGH);
  delay(100);
  }
 

   delay(intervalo);

   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Inflacion Exp.");
  lcd.setCursor(0, 1);
  lcd.print(ultima_fecha_inflacion);
  lcd.setCursor(11, 1);
  lcd.print(inflacion);
  lcd.setCursor(15, 1);
  lcd.print("%");

  delay(intervalo);
  
   lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("IPC Mensual");
  lcd.setCursor(0, 1);
  lcd.print(ultima_fecha_ipc);
  lcd.setCursor(11, 1);
  lcd.print(ipc);
  
  delay(intervalo);
  
   lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Can.Bas-dif-1ano");
  lcd.setCursor(0, 1);
  lcd.print(ultima_fecha_canasta);
  lcd.setCursor(11, 1);
  lcd.print(canasta_dif);
lcd.setCursor(15, 1);
lcd.print("%");

delay(intervalo);
lcd.clear();
  lcd.setCursor(0,0);
    lcd.print("Canasta Basica");
  lcd.setCursor(0, 1);
  lcd.print(ultima_fecha_canasta);
  lcd.setCursor(12, 1);
  lcd.print(canasta_pesos);
  lcd.setCursor(11, 1);
  lcd.print("$");

  delay(intervalo);

}

///////////// PUTE LE QUE LEE /////////



