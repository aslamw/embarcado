#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include "sites.h"

//ponto de wifi
const char* ssid = "GEL";
const char* password = "12345678";

//servidor DNS
const byte DNS_PORT = 53;
DNSServer dnsServer;

ESP8266WebServer server(80);

void handleRoot();

void setup() {
  Serial.begin(115200);
  
  WiFi.softAP(ssid, password);

  //inicia servidor DNS para direcionar tudo para o ip do esp
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  server.on("/generate_204", handleRoot);//android
  server.on("/redirect", handleRoot);//ios
  server.on("/hostpot-detect.html", handleRoot);//apple
  server.on("/ncsi.txt", handleRoot); // Teste do Windows NCSI
  server.on("/json", handleRoot); // Teste do Android TV
  server.on("/success.html", handleRoot); // Teste de portais
  server.on("/connectivity-check.html", handleRoot); // Outros sistemas

  server.onNotFound(handleRoot);
  server.on("/", handleRoot);

  //iniciar server
  server.begin();


}

void loop() {

  //mantém o servidor DNS e o web rodando
  dnsServer.processNextRequest();
  server.handleClient();

}
void handleRoot() {
  // Envia a página HTML para o navegador
  server.send(200, "text/html", HOME);
}