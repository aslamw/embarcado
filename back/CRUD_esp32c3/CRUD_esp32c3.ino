#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

// Configuração do AP (Access Point)
const char* ssid = "ESP32C3-base A";  // Nome da rede Wi-Fi criada pelo ESP
const char* password = "12345678"; // Senha (mínimo 8 caracteres)

WebServer server(80);

// Banco de dados simulado
const int MAX_RECORDS = 10;
DynamicJsonDocument records[MAX_RECORDS] = {
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024),
  DynamicJsonDocument(1024)
};
int recordCount = 0;

void criar() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Erro: Dados JSON ausentes");
    return;
  }
  
  if (recordCount >= MAX_RECORDS) {
    server.send(507, "text/plain", "Erro: Limite de registros atingido");
    return;
  }
  
  DeserializationError error = deserializeJson(records[recordCount], server.arg("plain"));
  if (error) {
    server.send(400, "text/plain", "Erro: JSON inválido");
    return;
  }
  
  recordCount++;
  server.send(201, "text/plain", "Registro criado com sucesso");
}

void ler() {
  DynamicJsonDocument doc(4096); // Tamanho suficiente para todos os registros
  JsonArray array = doc.to<JsonArray>();
  
  for (int i = 0; i < recordCount; i++) {
    array.add(records[i].as<JsonObject>());
  }
  
  String resposta;
  serializeJson(doc, resposta);
  server.send(200, "application/json", resposta);
}

void atualizar() {
  if (!server.hasArg("plain")) {
    server.send(400, "text/plain", "Erro: Dados JSON ausentes");
    return;
  }
  
  if (!server.hasArg("id")) {
    server.send(400, "text/plain", "Erro: Parâmetro 'id' ausente");
    return;
  }
  
  int id = server.arg("id").toInt();
  if (id < 0 || id >= recordCount) {
    server.send(404, "text/plain", "Erro: Registro não encontrado");
    return;
  }
  
  DeserializationError error = deserializeJson(records[id], server.arg("plain"));
  if (error) {
    server.send(400, "text/plain", "Erro: JSON inválido");
    return;
  }
  
  server.send(200, "text/plain", "Registro atualizado com sucesso");
}

void deletar() {
  if (!server.hasArg("id")) {
    server.send(400, "text/plain", "Erro: Parâmetro 'id' ausente");
    return;
  }
  
  int id = server.arg("id").toInt();
  if (id < 0 || id >= recordCount) {
    server.send(404, "text/plain", "Erro: Registro não encontrado");
    return;
  }
  
  // Move os registros para preencher o espaço vazio
  for (int i = id; i < recordCount - 1; i++) {
    records[i] = records[i + 1];
  }
  
  recordCount--;
  server.send(200, "text/plain", "Registro deletado com sucesso");
}

void setup() {
  Serial.begin(115200);
  
  // Configura o ESP como Access Point
  WiFi.softAP(ssid, password);
  
  Serial.println("Access Point criado");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Configura as rotas
  server.on("/criar", HTTP_POST, criar);
  server.on("/ler", HTTP_GET, ler);
  server.on("/atualizar", HTTP_PUT, atualizar);
  server.on("/deletar", HTTP_DELETE, deletar);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}