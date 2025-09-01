#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "pages_web.h"

// Configurações do WiFi
const char* ssid = "Bloco Int";
const char* password = "12345678";
int buttonState = 0;

//tela


// Servidor Web na porta 80 e DNS na porta 53
IPAddress apIP(192,168,4,1);
ESP8266WebServer server(80);
DNSServer dnsServer;

// Variável para controlar o estado do servidor
bool serverEnabled = false;
bool captivePortalEnabled = true;
bool wifiAPEnabled = false;
bool wifiState = true;

//rotas
void handleRoot();
void handleNotFound();

//configuração do ponto de Wifi
void startWiFiAP();
void stopWiFiAP();

void setup() {

    pinMode(16, INPUT_PULLUP);
    // Iniciar ponto de acesso
    WiFi.softAP(ssid, password);
    
    // Iniciar servidor DNS para captura de portal
    dnsServer.start(53, "bloco.com", WiFi.softAPIP());

    //Define as rotas do servidor web
    server.on("/", handleRoot);

    //qualquer url sera direcionada para a principal
    server.onNotFound(handleNotFound);

    //inicia o servidor
    server.begin();
}

void loop() {
    /*
    buttonState = digitalRead(16);
    
    if (buttonState == LOW) {
      wifiState = wifiState ? false : true;
      delay(300);
    }
    
    wifiState ? startWiFiAP() : stopWiFiAP();
    */
    
    startWiFiAP();
}

//Funções das rotas
void handleRoot() {
  server.send(200, "text/html", welcomeAtiva);
}

void handleNotFound(){
  //redireciona para a home
  server.sendHeader("Location", String("http://") + apIP.toString(), true);
  server.send(302, "text/plain", "");
}

// Função para ligar o ponto de acesso WiFi
void startWiFiAP() {
  if (!wifiAPEnabled) {
    WiFi.softAP(ssid, password);
    wifiAPEnabled = true;

    dnsServer.processNextRequest();
    server.handleClient();

  } else {
    Serial.println("Ponto de WiFi já está ligado!");
  }
}

// Função para desligar o ponto de acesso WiFi
void stopWiFiAP() {
  if (wifiAPEnabled) {

    server.close();

    WiFi.softAPdisconnect(true);
    wifiAPEnabled = false;

  } else {
    Serial.println("Ponto de WiFi já está desligado!");
  }
}