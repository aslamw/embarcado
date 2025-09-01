#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <page_home.h>
#include <text_page.h>

#define SSID "portal_esp32"
#define PASS "12345678"

//Endereço Ip modo AP
IPAddress apIP(192,168,4,1);
DNSServer dnsServer;
WebServer server(80);


// A função handleRoot() chama server.send(..., htmlRoot)
void handleRoot() {
  server.send(200, "text/html", htmlRoot);
}

// A função handleAbout() chama server.send(..., htmlAbout)
void handleAbout() {
  server.send(200, "text/html", htmlAbout);
}

void handleNotFound(){
  //redireciona para a home
  server.sendHeader("Location", String("http://") + apIP.toString(), true);
  server.send(302, "text/plain", "");
}

void setup() {
  
  //configura IP estático
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(SSID, PASS);

  //iniciar servidor DNS
  dnsServer.start(53, "*", apIP);


  //Define as rotas do servidor web
  server.on("/", handleRoot);
  server.on("/sobre", handleAbout);

  //qualquer url sera direcionada para a principal
  server.onNotFound(handleNotFound);

  //inicia o servidor
  server.begin();

}

void loop() {

  dnsServer.processNextRequest();
  server.handleClient();

}
