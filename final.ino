#include "ESP8266WiFi.h"
#include<WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "arduinoFFT.h"
#include <ThingerESP8266.h>

#define user "danushgupta21"
#define device_Id "traffic"
#define device_credentials "traffic"
ThingerESP8266 thing(user, device_Id, device_credentials);

int readings[2]={0};
const char* ssid = "Danush";
const char* password = "012345678";

WiFiServer server(80);

String header;

String output1State = "no";
String output2State = "no";
String output3State = "no";
String output4State = "no";
String output5State = "no";
String output6State = "no";

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
int val=0;

#define BOT_TOKEN "5353317373:AAHB-vBq6Ovk44_-fx_4LFRzhL5pNu5zhYE"
#define CHAT_ID "1063440679"
#define SAMPLES 64
#define SAMPLING_FREQUENCY 2048

WiFiClientSecure client;

arduinoFFT FFT = arduinoFFT();
unsigned int samplingPeriod;
unsigned long microSeconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
double vReal1[SAMPLES];
double vImag1[SAMPLES];

void setup(void)
{ 
  Serial.begin(115200);
  pinMode(D8,OUTPUT);//Green
  pinMode(D7,OUTPUT);//Red
  pinMode(D6,OUTPUT);//Yellow
  pinMode(D4,INPUT);//IR
  samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  server.begin();


  thing.add_wifi(ssid,password);
  thing["sound1"] >> outputValue(readings[0]);
  thing["sound2"] >> outputValue(readings[1]);
}

void loop() {
    digitalWrite(D6,HIGH);
    delay(2000);
    digitalWrite(D6,LOW);
    digitalWrite(D8,HIGH);//Green on
    digitalWrite(D7,LOW);
    delay(5000);

    digitalWrite(D6,HIGH);
    delay(2000);
    digitalWrite(D6,LOW);
    digitalWrite(D8,LOW);
    digitalWrite(D7,HIGH);//Red on
    delay(2000);
    
    for(int i=0; i<SAMPLES; i++)
    {
      microSeconds = micros();
      vReal[i] = analogRead(A0);
      vImag[i] = 0;
      
      vReal1[i] = analogRead(A0);
      vImag1[i] = 0;
       
      while(micros() < (microSeconds + samplingPeriod))
      {
      }
    }
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);

    FFT.Windowing(vReal1, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal1, vImag1, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal1, vImag1, SAMPLES);
    double peak1 = FFT.MajorPeak(vReal1, SAMPLES, SAMPLING_FREQUENCY);
    
    Serial.print("Frequency 1 Detected: ");
    Serial.println(peak);
    Serial.print("Frequency 2 Detected: ");
    Serial.println(peak1);

    WiFiClient client = server.available();

    if (client) {
      String currentLine = "";
      currentTime = millis();
      previousTime = currentTime;
      while (client.connected() && currentTime - previousTime <= timeoutTime) {
        currentTime = millis();         
        if (client.available()) {
          char c = client.read();
          header += c;
          if (c == '\n') {
            if (currentLine.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
            
              if (header.indexOf("GET /1/yes") >= 0) {
                output1State = "no";
              }
              else if (header.indexOf("GET /1/no") >= 0) {
                output1State = "yes";
              }
              else if (header.indexOf("GET /2/yes") >= 0) {
                output2State = "no";
              }
              else if (header.indexOf("GET /2/no") >= 0) {
                output2State = "yes";
              }
              else if (header.indexOf("GET /3/yes") >= 0) {
                output3State = "no";
              }
              else if (header.indexOf("GET /3/no") >= 0) {
                output3State = "yes";
              }
              else if (header.indexOf("GET /4/yes") >= 0) {
                output4State = "no";
              }
              else if (header.indexOf("GET /4/no") >= 0) {
                output4State = "yes";
              }
              else if (header.indexOf("GET /5/yes") >= 0) {
                output5State = "no";
              }
              else if (header.indexOf("GET /5/no") >= 0) {
                output5State = "yes";
              }
              else if (header.indexOf("GET /6/yes") >= 0) {
                output6State = "no";
              }
              else if (header.indexOf("GET /6/no") >= 0) {
                output6State = "yes";
              }
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println(".button2 {background-color: #77878A;}</style></head>");
             
              client.println("<body><h1>Patient Emergency Situation</h1>");
    
              if (output1State=="yes") {
                client.println("<p><a href=\"/1/yes\"><button class=\"button\">Heart Attack</button></a></p>");
              }
              else {
                client.println("<p><a href=\"/1/no\"><button class=\"button button2\">Heart Attack</button></a></p>");
              } 
                     
              if (output2State=="yes") {
                client.println("<p><a href=\"/2/yes\"><button class=\"button\">Stroke</button></a></p>");
              }
              else {
                client.println("<p><a href=\"/2/no\"><button class=\"button button2\">Stroke</button></a></p>");
              } 
      
              if (output3State=="yes") {
                client.println("<p><a href=\"/3/yes\"><button class=\"button\">Breathing Difficulties</button></a></p>");
              }
              else {
                client.println("<p><a href=\"/3/no\"><button class=\"button button2\">Breathing Difficulties</button></a></p>");
              } 
      
              if (output4State=="yes") {
                client.println("<p><a href=\"/4/yes\"><button class=\"button\">Epileptic Seizure</button></a></p>");
              }
              else {
                client.println("<p><a href=\"/4/no\"><button class=\"button button2\">Epileptic Seizure</button></a></p>");
              } 
      
              if (output5State=="yes") {
                client.println("<p><a href=\"/5/yes\"><button class=\"button\">Bleeding</button></a></p>");
              }
              else {
                client.println("<p><a href=\"/5/no\"><button class=\"button button2\">Bleeding</button></a></p>");
              }
                   
              if (output6State=="yes") {
                client.println("<p><a href=\"/6/yes\"><button class=\"button\">Severe Pain</button></a></p>");
              }
              else {
                client.println("<p><a href=\"/6/no\"><button class=\"button button2\">Severe Pain</button></a></p>");
              } 
            
              client.println("</body></html>");
            
              client.println();
              break;
            }
            else {
              currentLine = "";
            }
         }
         else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("");
  }
    
  if((peak>700 || peak1>700)&&(output1State=="yes"||
  output2State=="yes"||output3State=="yes"||output4State=="yes"||
  output5State=="yes"||output6State=="yes"))
  {
    digitalWrite(D6,HIGH);
    delay(2000);
    digitalWrite(D6,LOW);
    digitalWrite(D8,HIGH);//Green on
    digitalWrite(D7,LOW);
    Serial.println("EMERGENCY VEHICLE DETECTED! GREEN LIGHT");
    delay(5000);
   }
   else
   {
    val = digitalRead(D4); // read input value 
    if (val == 1)
    { // check if the input is HIGH
      Serial.println("Reasonable Traffic present");
      digitalWrite(D8, LOW);
      digitalWrite(D6,HIGH);
      delay(2000);
      digitalWrite(D6,LOW);
      digitalWrite(D7, HIGH);//Red on 
      delay(5000); 
   } 
   else 
   { 
      Serial.println("High Traffic present");
      digitalWrite(D7, LOW);
      digitalWrite(D6,HIGH);
      delay(2000);
      digitalWrite(D6,LOW);
      digitalWrite(D8, HIGH);//Green on
      delay(5000);
   }
   }
   delay(5000);
   readings[0]=peak;
   readings[1]=peak1;
   thing.handle();
   digitalWrite(D8,LOW);
   digitalWrite(D6,HIGH);
   delay(2000);
   digitalWrite(D6,LOW);
   digitalWrite(D7,HIGH);//Red on
   delay(5000);
}
