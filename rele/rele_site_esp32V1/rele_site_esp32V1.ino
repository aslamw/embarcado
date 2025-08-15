
//ver se tem ACS712 na empresa
#include <WiFi.h>
#include <WebServer.h>

// Configurações do ponto de acesso
const char* ssid = "ESP32-Relé";
const char* password = "12345678"; // Senha do AP

// Pino do relé
const int relayPin = 2; // Altere para o pino que você está usando

WebServer server(80);

void handleRoot() {
  String html = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1'>"
                "<title>Controle de Relé</title>"
                "<style>"
                "body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }"
                ".btn {"
                "  background-color: #4CAF50;"
                "  border: none;"
                "  color: white;"
                "  padding: 15px 32px;"
                "  text-align: center;"
                "  text-decoration: none;"
                "  display: inline-block;"
                "  font-size: 16px;"
                "  margin: 10px 5px;"
                "  cursor: pointer;"
                "  border-radius: 5px;"
                "}"
                ".btn-off { background-color: #f44336; }"
                ".status { font-size: 20px; margin: 20px; }"
                "</style>"
                "</head>"
                "<body>"
                "<h1>Controle de Relé</h1>"
                "<div class='status'>Status: <strong>" + String(digitalRead(relayPin) ? "LIGADO" : "DESLIGADO") + "</strong></div>"
                "<a href='/on' class='btn'>Ligar Relé</a>"
                "<a href='/off' class='btn btn-off'>Desligar Relé</a>"
                "</body>"
                "</html>";
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(relayPin, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  digitalWrite(relayPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  
  // Configura o pino do relé
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Inicia com relé desligado

  // Cria o ponto de acesso
  WiFi.softAP(ssid, password);
  Serial.println("Ponto de acesso criado");
  Serial.print("IP para acesso: ");
  Serial.println(WiFi.softAPIP());

  // Configura as rotas do servidor web
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  // Inicia o servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}