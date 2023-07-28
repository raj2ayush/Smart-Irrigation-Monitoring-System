#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <ESP32Servo.h>


#define API_KEY "AIzaSyB8p0lXZjDhIKSQXD9GYsJHoOq5W86Zb40"

#define DATABASE_URL "https://enthalpy2-1c38d-default-rtdb.asia-southeast1.firebasedatabase.app"
const char* ssid = "abcd";
const char* password = "password";
const char* wssid = "Cooler is Freezer";
const char* wpassword =  "freezeriscoolers";
FirebaseData fbdo;
int sensor_pin = 2 ;
int value ;
int motor;
float ph;
FirebaseAuth auth;
FirebaseConfig config;
Servo myservo;
int servoPin = 2;
int pos = 0;
//const char* serverNameHumi = "http://192.168.4.1/humidity";

WiFiServer server(80);
String humidity;
String request;
bool signupOK = false;

void setup() {
 
 WiFi.mode(WIFI_MODE_APSTA);
  Serial.begin(9600);
  WiFi.softAP(ssid, password);

  WiFi.begin(wssid, wpassword);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println  (myIP ) ;

   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
 Serial.print("ESP32 IP as soft AP: ");
 Serial.println(WiFi.softAPIP());
 
  Serial.print("ESP32 IP on the WiFi network: ");
  Serial.println(WiFi.localIP());
 // server.on("/humidity", handle_humidity);
  //server.onNotFound(handle_NotFound);

  server.begin();
config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
	myservo.setPeriodHertz(50);
	myservo.attach(servoPin, 1000, 2000);

}

void loop() {



//server.handleClient();
float h = 83.81;
WiFiClient client = server.available();
if (client) { 
  Serial.println("New Client.");
  Serial.println(client.connected());
  Serial.println(client.available());
  while (client.connected() ) {
    if (client.available()) {
      Serial.println("accessed") ;
      humidity = client.readStringUntil('\r');
      Serial.println(humidity);
      String str = humidity;
String strs[20];
int StringCount = 0;

  while (str.length() > 0)
  {
    int index = str.indexOf(' ');
    if (index == -1) // No space found
    {
      strs[StringCount++] = str;
      break;
    }
    else
    {
      strs[StringCount++] = str.substring(0, index);
      str = str.substring(index+1);
    }
  }
  float tc,tf,hc,hf,m;
h = strs[0].toFloat();
tc = strs[1].toFloat();
tf = strs[2].toFloat();
hc = strs[3].toFloat();
hf = strs[4].toFloat();
m = strs[5].toFloat();
ph = strs[6].toFloat();
Serial.println("****************************");
Serial.println( h );
  

 if (h != 0.00) {
      if (Firebase.ready() && signupOK ) {
       if (Firebase.RTDB.setFloat(&fbdo, "test/humidity", h)) {
         Serial.println ( " Put in firebase " ) ;
       }
       else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
       if (Firebase.RTDB.setFloat(&fbdo, "test/temperature_in_C",tc)) {
         Serial.println ( " Put in firebase " ) ;
       }
       else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }

       if (Firebase.RTDB.setFloat(&fbdo, "test/temperature_in_F", tf)) {
         Serial.println ( " Put in firebase " ) ;
       }
       else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
       if (Firebase.RTDB.setFloat(&fbdo, "test/heatindex_in_C", hc)) {
         Serial.println ( " Put in firebase " ) ;
       }
       else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
      //  if (Firebase.RTDB.setFloat(&fbdo, "test/heatindex_in_F", hf)) {
      //    Serial.println ( " Put in firebase " ) ;
      //  }
      //  else {
      //   Serial.println("FAILED");
      //   Serial.println("REASON: " + fbdo.errorReason());
      // }
       if (Firebase.RTDB.setFloat(&fbdo, "test/moisture", m)) {
         Serial.println ( " Put in firebase " ) ;
       }
       else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
     if (Firebase.RTDB.getInt(&fbdo, "test/motor")) {
       motor=fbdo.intData();
         Serial.println (motor) ;
       }
       else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
          if (Firebase.RTDB.setFloat(&fbdo, "test/phValue", ph)) {
         Serial.println ( " Put in firebase " ) ;
       }
       else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
      }


 }



if (motor == 1) {
	for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
		// in steps of 1 degree
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15); 
                // waits 15ms for the servo to reach the position
	}
	for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
		delay(15);             // waits 15ms for the servo to reach the position
	}



}

    }
  }

}


Serial.println(" next");
delay (2000);




}



void handle_humidity () {

//server.send(200, "text/html", "Data received");
Serial.println( " accsses from client " ) ;


}
