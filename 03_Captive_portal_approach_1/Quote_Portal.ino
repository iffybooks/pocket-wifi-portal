#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String responseHTML = "<!DOCTYPE html>"
"<html lang='en'>"
"<head>"
"<meta charset='utf-8'>"
"<meta name='viewport' content='width=device-width, initial-scale=1'>"
"<title>Free Reading Wi-fi Portal</title>"
"<style>"
"p {font-family: Georgia, serif; font-size: 200%; text-align: center; padding-top: 50px;}"
"div {width: 75%; margin: 0 auto;}"
"</style>"
"</head>"
"<body bgcolor='lightpink'>"
"<div>"
"<p>\"What we do is more important than what we say or what we say we believe.\" —bell hooks</p>"
"<p><img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAMAAAAoyzS7AAAABlBMVEV/01gAAACf/yQfAAAADElEQVR42mJgAAgwAAACAAFPbVnhAAAAAElFTkSuQmCC' alt='high-contrast photo of bell hooks' /></p>"
"</div>"
"</body>"
"</html>";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Free Reading Wifi");
  dnsServer.start(DNS_PORT, "*", apIP);
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  });
  webServer.begin();
}

void loop() {
 if (WiFi.softAPgetStationNum() == 0) 
 {
    delay(100);
 } else {
    delay(100);
    dnsServer.processNextRequest();
    webServer.handleClient();
 }
}