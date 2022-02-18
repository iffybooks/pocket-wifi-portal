// This part is still a work in progress! I'll update it before Saturday 2/19.
// Captive portal with (arduino) OTA + SPIFFS
// Adapted from https://git.vvvvvvaria.org/then/ESP8266-captive-ota-spiffs

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include "./DNSServer.h" // Dns server
#include <FS.h> // SPIFFS

DNSServer dnsServer;
const byte DNS_PORT = 53;

ESP8266WebServer server(80);

#ifndef STASSID
#define STASSID "Free Reading Wifi"
#endif

IPAddress apIP(192, 168, 4, 1);
const char* ssid = STASSID;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid);
  dnsServer.start(DNS_PORT, "*", apIP); // redirect dns request to AP ip
  
  MDNS.begin("esp8266", WiFi.softAPIP());
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  //File system begin
  SPIFFS.begin();


   //redirect all traffic to index.html
server.onNotFound([]() {
  if(!handleFileRead(server.uri())){
    const char *metaRefreshStr = "<head><meta http-equiv=\"refresh\" content=\"0; url=http://192.168.4.1/index.html\" /></head><body><p>redirecting...</p></body>";
    server.send(200, "text/html", metaRefreshStr);
  }
});

  server.begin();

}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  delay(50);
}


String getContentType(String filename){
if(server.hasArg("download")) return "application/octet-stream";
else if(filename.endsWith(".htm")) return "text/html";
else if(filename.endsWith(".html")) return "text/html";
else if(filename.endsWith(".css")) return "text/css";
else if(filename.endsWith(".js")) return "application/javascript";
else if(filename.endsWith(".png")) return "image/png";
else if(filename.endsWith(".gif")) return "image/gif";
else if(filename.endsWith(".jpg")) return "image/jpeg";
else if(filename.endsWith(".ico")) return "image/x-icon";
else if(filename.endsWith(".xml")) return "text/xml";
else if(filename.endsWith(".mp4")) return "video/mp4";
else if(filename.endsWith(".pdf")) return "application/x-pdf";
else if(filename.endsWith(".zip")) return "application/x-zip";
else if(filename.endsWith(".gz")) return "application/x-gzip";
return "text/plain";
}

//Given a file path, look for it in the SPIFFS file storage. Returns true if found, returns false if not found.
bool handleFileRead(String path){
if(path.endsWith("/")) path += "index.html";
String contentType = getContentType(path);
String pathWithGz = path + ".gz";
if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
  if(SPIFFS.exists(pathWithGz))
    path += ".gz";
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, contentType);
  file.close();
  return true;
}
return false;
}