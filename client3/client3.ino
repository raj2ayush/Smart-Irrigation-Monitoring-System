#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
// #include <RH_ASK.h>
//  #include <RCSwitch.h>
#include <SPI.h> 

#define DHTPIN 15     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22
// RCSwitch mySwitch = RCSwitch();
DHT dht(DHTPIN, DHTTYPE);
// RH_ASK rf_driver;
const char* ssid = "abcd";
const char* password="password";
const int analogInPin = 13; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
const char* serverNameHumi = "http://192.168.4.1/humidity";
int sensor_pin = 2 ;
int value ;
String data;
float h;
float t;
float f;
float hic;
float hif;

void setup() {
 dht.begin();
Serial.begin(9600);
 //rf_driver.init();
//  mySwitch.enableTransmit(0);  
}

void loop() {

Serial.println("Printing");
h = dht.readHumidity();
t = dht.readTemperature();
f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      //timeSinceLastRead = 0;
      return;
    }
  hif = dht.computeHeatIndex(f, h);
  hic = dht.computeHeatIndex(t, h, false);

 /* Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.print(" *C ");
    Serial.print(hif);
    Serial.println(" *F");
*/
  value= analogRead(sensor_pin);
  value = map(value,550,0,0,100);
  Serial.print("Moisture : ");
  Serial.print(value);
  Serial.println("%");
//   mySwitch.send(1394007, 24);
// mySwitch.send(1394006, 24);
/*const char *msg = "Emitting Frequency";
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();*/
    for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(100);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + 8.65;
   data = h;
   data += " ";
   data += t;
   data += " ";
   data += f;
   data += " ";
   data += t;
   data += " ";
   data += hic;
   data += " ";
   data += hif;
   data += "\r";
   data += " ";
   data += value;
  data += " ";
   data += phValue;
Serial.println( data );

  WiFiClient client;
  const char* host = "192.168.4.1";
  const int httpPort = 80;
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

 
   
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  else { 
    Serial.println("connection success");
    client.println( data );

  }
  delay(500);

 /*   HTTPClient http;
    String serverPath = "http://192.168.4.1:80/humidity" ;
    http.begin(serverPath.c_str());
          int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();

  }
*/
 //client.print(String("GET ") + /

delay (2000);

}