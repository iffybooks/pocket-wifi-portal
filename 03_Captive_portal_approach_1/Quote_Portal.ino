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
"<p><img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAHIAAAB/CAMAAAAEnWaRAAAABlBMVEX///8AAABVwtN+AAADaUlEQVR42uyb647VMAyEx+//0ggdTjdNfRlfUoREhcSPXfLFjuOMnQB5/cP7Q6HJWP49cBCJz5/vlxysgPwNwfpds8BJx67Q7Bio/OJmJXKrmUBidS12KOjhkF5IBbhF0SgS/se5NrV/gQkmylujzMSYV+9x1EWCJmLSsTwSI1aCJ8ZMjBPD5cSsV/+YideR0kQiSwRGwgeDnqWsBCbNxAG/oh2xKHzicBHNBrXvq8nesxLO2NEOQhmJErJhJV4Pn09yL63l7ldlCsak7MEJB2h61Z/FNZViKlDXx51EsJhg/CqWnc5qF8MH2/m8W+nGVxGJy9LvyLec5AZ0J61jK3BWs2GrXLMuouWWtnMeYSa7lVnk5Tlduz9/su2YRvaxdo6ChLaDMlYqc7/5VbNnSUdFK6HJ2AdyOT3W6C4hzSPfLrw2o9OODQjxnsSMlTHTHR0nzsoLWkkFiXWkD0wemS3AzCr+lGOdcqiA7PYNUqrg4adSRc0cXs4hWJGWSVGZXG01gNAk+nJeBaBJNPuI9tAEUiKkeIFTceyP0oPhBMkJrkTX/6F1rr+G9yURoUn1jD7R7CYYNSb6RNO1HR0b7XvL4egjFRG51An79YlgAGnp2ZuankMitFxb1EyOVVxIOFuthRrN0YpIGkZqhTyTgJDZHKZOmD6iH4W6cJmgi7y7Nl8oVJF7Ne12tkz1jGyuI4nOyJk7r/sxfbadf5ckoaqbvTL9cNUcs58eE44Nt8LWEMV55IkHG8zyYRbZvmsvChF5GXnrJNWADSFSJk4+MfqPPIjEX7CSeIPzz1tJJT8ME9927O0gM9/hYJgIUyqdQG6Vj5n7DyB/yj4R9SpyzPL7PcKlkxi1iR7xUs22jrcjoIkUs40wcAA+kEuXjevP94iblUNPzCImck/aqkTxWkFyGrlkdzBFDJDagpSd0nasVRpU7zZCpNalFHugeSKcC8chZPrO64CRfjuEuwksNJ27zFHHUsguUfKLGb1TwXQAEaoX4j4sSieDWBIErSYtNRHPysYerQn7jG/w/apQ7/n54wvE8/lwciW51Q2f5CGd2qRDugBjVLLaifZ6KtBAlK1xeomRS4048f8vvp1SMqLZu3/xb/iJmP2Gg2AASSz2/SWYUFdcUceU3privW59ML3IUH/jlwADAFRWI+4MU5SBAAAAAElFTkSuQmCC' alt='high-contrast photo of bell hooks' /></p>"
"</div>"
"</body>"
"</html>";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("bell hooks quote");
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