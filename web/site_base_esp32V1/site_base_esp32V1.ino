//controle do wifi
#include <WiFi.h>
#include <WebServer.h>

//controle de rota de acesso
#include <DNSServer.h>

//para dispositivos apple
#include <ESPmDNS.h>

//configurar acesso
const char* ssid = "ESP32_bad";
const char* password = "12345678";

//configurar servidor
WebServer server(80);

DNSServer dnsServer;

//pagina html
const char* pageHome = R"rawliteral(
  <!DOCTYPE html>
  <html lang="pt-br">
  <head>
    <meta charset="UTF-8"/>
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <title> teste </test>
    <style>
      body {
        font-family: Arial, sans-serif;
        text-align: center;
        margin-top: 50px;
        background-color: #f0f0f0;
      }
      h1 {
        color: #333;
      }
    </style>
  </head>
  <body>
    <h1> hello world!</h1>
  </body>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", pageHome);
}

void setup() {
  Serial.begin(115200);

  //configurando ap
  WiFi.softAP(ssid,password);
  // Configura o captive portal
  dnsServer.start(53, "*", WiFi.softAPIP(ssid,password));
  IPAddress IP = WiFi.softAPIP();//pega IP
  Serial.print("IP: ");
  Serial.println(IP);

  if (!MDNS.begin("esp32")) {
  Serial.println("Erro ao iniciar mDNS");
  }
  MDNS.addService("http", "tcp", 80);

  server.on("/", handleRoot);
  // Adicione isso junto com as outras rotas no setup()
  server.on("/generate_204", HTTP_GET, []() {  // Android captive portal
    server.sendHeader("Location", "http://"+server.client().localIP().toString());
    server.send(302, "text/plain", "");
  });

  server.on("/fwlink", HTTP_GET, []() {  // Microsoft captive portal
    server.sendHeader("Location", "http://"+server.client().localIP().toString());
    server.send(302, "text/plain", "");
  });

  server.begin();
  Serial.println("servidor HTTP iniciado");


}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}


