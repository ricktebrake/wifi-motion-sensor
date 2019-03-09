#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char ssid[] = "aitisal-way-faaya-laysa-minak";
const char password[] = "nomnomnom88";
const char* host = "us-central1-clean-176514.cloudfunctions.net";
//const char* host = "www.google.com";
const char* url = "/esp-test";
const int httpsPort = 443;

const char* root_ca= \
  "-----BEGIN CERTIFICATE-----\n" \
"MIIEXDCCA0SgAwIBAgINAeOpMBz8cgY4P5pTHTANBgkqhkiG9w0BAQsFADBMMSAw\n" \
"HgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEGA1UEChMKR2xvYmFs\n" \
"U2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjAeFw0xNzA2MTUwMDAwNDJaFw0yMTEy\n" \
"MTUwMDAwNDJaMFQxCzAJBgNVBAYTAlVTMR4wHAYDVQQKExVHb29nbGUgVHJ1c3Qg\n" \
"U2VydmljZXMxJTAjBgNVBAMTHEdvb2dsZSBJbnRlcm5ldCBBdXRob3JpdHkgRzMw\n" \
"ggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDKUkvqHv/OJGuo2nIYaNVW\n" \
"XQ5IWi01CXZaz6TIHLGp/lOJ+600/4hbn7vn6AAB3DVzdQOts7G5pH0rJnnOFUAK\n" \
"71G4nzKMfHCGUksW/mona+Y2emJQ2N+aicwJKetPKRSIgAuPOB6Aahh8Hb2XO3h9\n" \
"RUk2T0HNouB2VzxoMXlkyW7XUR5mw6JkLHnA52XDVoRTWkNty5oCINLvGmnRsJ1z\n" \
"ouAqYGVQMc/7sy+/EYhALrVJEA8KbtyX+r8snwU5C1hUrwaW6MWOARa8qBpNQcWT\n" \
"kaIeoYvy/sGIJEmjR0vFEwHdp1cSaWIr6/4g72n7OqXwfinu7ZYW97EfoOSQJeAz\n" \
"AgMBAAGjggEzMIIBLzAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0lBBYwFAYIKwYBBQUH\n" \
"AwEGCCsGAQUFBwMCMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFHfCuFCa\n" \
"Z3Z2sS3ChtCDoH6mfrpLMB8GA1UdIwQYMBaAFJviB1dnHB7AagbeWbSaLd/cGYYu\n" \
"MDUGCCsGAQUFBwEBBCkwJzAlBggrBgEFBQcwAYYZaHR0cDovL29jc3AucGtpLmdv\n" \
"b2cvZ3NyMjAyBgNVHR8EKzApMCegJaAjhiFodHRwOi8vY3JsLnBraS5nb29nL2dz\n" \
"cjIvZ3NyMi5jcmwwPwYDVR0gBDgwNjA0BgZngQwBAgIwKjAoBggrBgEFBQcCARYc\n" \
"aHR0cHM6Ly9wa2kuZ29vZy9yZXBvc2l0b3J5LzANBgkqhkiG9w0BAQsFAAOCAQEA\n" \
"HLeJluRT7bvs26gyAZ8so81trUISd7O45skDUmAge1cnxhG1P2cNmSxbWsoiCt2e\n" \
"ux9LSD+PAj2LIYRFHW31/6xoic1k4tbWXkDCjir37xTTNqRAMPUyFRWSdvt+nlPq\n" \
"wnb8Oa2I/maSJukcxDjNSfpDh/Bd1lZNgdd/8cLdsE3+wypufJ9uXO1iQpnh9zbu\n" \
"FIwsIONGl1p3A8CgxkqI/UAih3JaGOqcpcdaCIzkBaR9uYQ1X4k2Vg5APRLouzVy\n" \
"7a8IVk6wuy6pm+T7HT4LY8ibS5FEZlfAFLSW8NwsVz9SBK2Vqn1N0PIMn5xA6NZV\n" \
"c7o835DLAFshEWfC7TIe3g==\n" \
"-----END CERTIFICATE-----\n";

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
  int moistureValue = analogRead(A0);
  int motionSensorValue = digitalRead(D2);
  String init = "";
  String message = init + "{\n" +
  "\"motion-sensor\":\"" + motionSensorValue + "\",\n" +
  "\"soil-sensor\": \"" + moistureValue + "\"\n" +
  "}";


  WiFiClientSecure secureClient;
  secureClient.setFingerprint("F5:7E:50:0A:29:CF:2D:08:34:E2:51:F2:9A:C8:AE:9D:DA:42:5F:A2");
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
    // while(secureClient.connected()) {
    //   if(secureClient.available()) {
    //     String line = secureClient.readStringUntil('\n');
    //     if(line == "\r") {
    //       printNext = true;
    //     }
    //     if(printNext && line != "\r" || line != "\n") {
    //       Serial.println(line);
    //       Serial.println();
    //     }
    //   }
    // }
    secureClient.stop();

    Serial.println("Connection closed");
    // if(motionSensorValue == 0) {
    //
    // } else if(motionSensorValue == 1) {
    //
    // }
    delay(600000);
  } else {
    Serial.println("Connection failed");
    secureClient.stop();
    return;
  }
}
