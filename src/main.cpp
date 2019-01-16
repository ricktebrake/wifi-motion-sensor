#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char ssid[] = "NSA Public Wifi";
const char password[] = "nomnomnom88";
const char* host = "us-central1-clean-176514.cloudfunctions.net";
//const char* host = "www.google.com";
const char* url = "/esp-test";
const int httpsPort = 443;

const char* fingerprint = "58 19 DC B7 72 46 C5 B4 B9 11 23 E4 43 18 80 FF D3 F3 BC 4E";

int motionSensorValue;

void setup() {
  Serial.begin(115200);

  pinMode(D1, OUTPUT);
  pinMode(D2, INPUT);

  WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(D1, HIGH);
    delay(250);
    Serial.print(".");
    digitalWrite(D1, LOW);
    delay(250);
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());



}

void loop() {
  motionSensorValue = digitalRead(D2);
  String message;
  if(motionSensorValue == 0) {
    message = "{\"message\":\"Nobody here :(\"}";
  } else {
    message = "{\"message\":\"Motion detected!\"}";
  }
  WiFiClientSecure secureClient;
  Serial.println("Connectiong to ");
  Serial.println(host);
  if(secureClient.connect(host, 443)) {

    Serial.println("Connected!");

    secureClient.print(String("POST ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: "+message.length()+"\r\n" +
                 "Connection: close\r\n" +
                 "\r\n" +
                 message);



    Serial.println("Request sent");
    unsigned long timeout = millis();
    bool printNext = false;
    while(secureClient.connected()) {
      if(secureClient.available()) {
        String line = secureClient.readStringUntil('\n');
        if(line == "\r") {
          printNext = true;
        }
        if(printNext && line != "\r" || line != "\n") {
          Serial.println(line);
          Serial.println();
        }
      }
    }
    secureClient.stop();

    Serial.println("Connection closed");
    // if(motionSensorValue == 0) {
    //   digitalWrite(D1, LOW);
    // } else if(motionSensorValue == 1) {
    //   digitalWrite(D1, HIGH);
    // }

  } else {
    Serial.println("Connection failed");
    secureClient.stop();
    return;
  }
  delay(1000);
}
