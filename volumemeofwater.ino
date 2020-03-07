
//Water Flow Sensor output processed to read in litres/hour
//Adaptation Courtesy: hobbytronics.co.uk

volatile int flow_frequency; // Measures flow sensor pulses
// Calculated litres/hour
 float vol = 0.0,l_minute;
#define  flowsensor D2 // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
#include <ESP8266WiFi.h>

const char* host = "api.thingspeak.com"; // Your domain

String ApiKey = " **************"; // the API key that has to select is write API

String path = "/update?key=" + ApiKey + "&field1=";

const char* ssid = "SSIDNAME";

const char* pass = "Password";
void flow () // Interrupt function
{
   flow_frequency++;
}
void setup()
{
  Serial.begin(115200);
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(115200);
   
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   currentTime = millis();
   cloopTime = currentTime;
 WiFi.begin(ssid, pass);

  // Wait for connection

  while (WiFi.status() != WL_CONNECTED) {

    delay(100);

    Serial.print(".");

  }
 
  Serial.println("");

  Serial.print("Connected to ");

  Serial.println(ssid);

  Serial.print("IP address: ");

  Serial.println(WiFi.localIP()); 
  
}
void loop ()
{
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
     
      //Serial.print("Rate: ");
      //Serial.print(l_minute);
      //Serial.print(" L/M");
      l_minute = l_minute/60;
      vol = vol +l_minute;
      Serial.print("Vol:");
      Serial.print(vol);
      Serial.print("L\n");
      flow_frequency = 0; // Reset Counter
     // Serial.print(l_minute, DEC); // Print litres/hour
      //Serial.println(" L/Sec \n");
    }
    else {
      //Serial.println(" flow rate = 0 ");
     
      //Serial.print("Rate: ");
      //Serial.print( flow_frequency );
      //Serial.print(" L/M");
      
      Serial.print("Vol:");
      Serial.print(vol);
      Serial.print("L\n");
    }
    WiFiClient client;

  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {

    Serial.println("connection failed");

    return;

  }

  client.print(String("GET ") + path + String(vol) + " HTTP/1.1\r\n" +

               "Host: " + host + "\r\n" +

               "Connection: keep-alive\r\n\r\n");

  delay(16000);
   }
}
