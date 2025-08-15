#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configurações do display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configurações da rede WiFi
const char* ssid = "esp32_s3";
const char* password = "12345678";

WebServer server(80);

// Estrutura de dados para armazenar itens
struct Item {
  int id;
  String name;
  String text;
};

// "Banco de dados" em memória
Item items[10];
int itemCount = 0;

void setup() {
  Serial.begin(115200);
  
  // Inicializa o display OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar display OLED"));
    for(;;); // Trava se não conseguir iniciar o display
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Iniciando...");
  display.display();
  delay(1000);

  // Inicializa o WiFi em modo AP (Access Point)
  WiFi.softAP(ssid, password);
  
  // Mostra informações no display
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Rede: " + String(ssid));
  display.println("IP: " + WiFi.softAPIP().toString());
  display.display();

  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Rotas do servidor
  server.on("/", HTTP_GET, handleRoot);
  server.on("/items", HTTP_GET, handleGetItems);
  server.on("/items", HTTP_POST, handlePostItem);
  server.on("/item", HTTP_GET, handleGetItem);
  server.on("/item", HTTP_PUT, handlePutItem);
  server.on("/item", HTTP_DELETE, handleDeleteItem);
  server.on("/items/search", HTTP_GET, handleSearchItems); // Nova rota de busca
  
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");

  // Atualiza display
  updateDisplay("Servidor iniciado");
}

void loop() {
  server.handleClient();
}

// Função para atualizar o display
void updateDisplay(const String &message) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Rede: " + String(ssid));
  display.println("Itens: " + String(itemCount));
  display.println(message);
  display.display();
}

// Funções auxiliares para gerar JSON manualmente
String itemToJson(const Item &item) {
  String json = "{";
  json += "\"id\":" + String(item.id) + ",";
  json += "\"name\":\"" + item.name + "\",";
  json += "\"text\":\"" + item.text + "\"";
  json += "}";
  return json;
}

String itemsArrayToJson(Item items[], int count) {
  String json = "[";
  for (int i = 0; i < count; i++) {
    if (i > 0) json += ",";
    json += itemToJson(items[i]);
  }
  json += "]";
  return json;
}

// Funções de manipulação das requisições

void handleRoot() {
  updateDisplay("Acesso raiz");
  String html = "<html><head><title>ESP32-S3 CRUD Server</title></head><body>";
  html += "<h1>Bem-vindo ao servidor CRUD do ESP32-S3</h1>";
  html += "<p>Endpoints disponíveis:</p>";
  html += "<ul>";
  html += "<li>GET /items - Listar todos os itens</li>";
  html += "<li>POST /items - Criar novo item (enviar JSON: {\"name\":\"...\",\"text\":\"...\"})</li>";
  html += "<li>GET /item?id=X - Obter um item específico</li>";
  html += "<li>PUT /item?id=X - Atualizar um item (enviar JSON)</li>";
  html += "<li>DELETE /item?id=X - Remover um item</li>";
  html += "<li>GET /items/search?key=name&value=marcos - Buscar itens por chave</li>";
  html += "</ul>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleGetItems() {
  updateDisplay("Listando itens");
  server.send(200, "application/json", itemsArrayToJson(items, itemCount));
}

void handleSearchItems() {
  if (!server.hasArg("key") || !server.hasArg("value")) {
    updateDisplay("Busca: params faltando");
    server.send(400, "text/plain", "Parâmetros key e value são obrigatórios");
    return;
  }

  String key = server.arg("key");
  String value = server.arg("value");
  
  // Filtra os itens que correspondem à busca
  Item matchedItems[10];
  int matchedCount = 0;
  
  for (int i = 0; i < itemCount; i++) {
    if (key == "name" && items[i].name.indexOf(value) != -1) {
      matchedItems[matchedCount] = items[i];
      matchedCount++;
    } else if (key == "text" && items[i].text.indexOf(value) != -1) {
      matchedItems[matchedCount] = items[i];
      matchedCount++;
    } else if (key == "id" && String(items[i].id) == value) {
      matchedItems[matchedCount] = items[i];
      matchedCount++;
    }
  }
  
  updateDisplay("Busca: " + key + "=" + value);
  server.send(200, "application/json", itemsArrayToJson(matchedItems, matchedCount));
}

void handlePostItem() {
  if (!server.hasArg("plain")) {
    updateDisplay("Erro: corpo vazio");
    server.send(400, "text/plain", "Corpo da requisição vazio");
    return;
  }
  
  String body = server.arg("plain");
  body.trim(); // Remove espaços extras
  
  // Parse mais robusto do JSON
  String name = extractJsonValue(body, "name");
  String text = extractJsonValue(body, "text");
  
  if (name == "" || text == "") {
    updateDisplay("Erro: JSON inválido");
    server.send(400, "text/plain", "Formato JSON inválido. Campos obrigatórios: name, text");
    return;
  }
  
  if (itemCount >= 10) {
    updateDisplay("Erro: limite itens");
    server.send(507, "text/plain", "Limite de itens atingido");
    return;
  }
  
  Item newItem;
  newItem.id = itemCount + 1;
  newItem.name = name;
  newItem.text = text;
  
  items[itemCount] = newItem;
  itemCount++;
  
  updateDisplay("Item criado: " + newItem.name);
  server.send(201, "application/json", "{\"id\":" + String(newItem.id) + "}");
}

// Função auxiliar para extrair valores do JSON de forma mais robusta
String extractJsonValue(String json, String key) {
  int keyPos = json.indexOf("\"" + key + "\":");
  if (keyPos == -1) return "";
  
  int start = json.indexOf("\"", keyPos + key.length() + 3) + 1;
  int end = json.indexOf("\"", start);
  
  if (start == 0 || end == -1) return "";
  
  return json.substring(start, end);
}

void handleGetItem() {
  if (!server.hasArg("id")) {
    updateDisplay("Erro: ID faltando");
    server.send(400, "text/plain", "Parâmetro id faltando");
    return;
  }
  
  int id = server.arg("id").toInt();
  
  if (id <= 0 || id > itemCount) {
    updateDisplay("Erro: item nao encontrado");
    server.send(404, "text/plain", "Item não encontrado");
    return;
  }
  
  updateDisplay("Consultando item " + String(id));
  server.send(200, "application/json", itemToJson(items[id-1]));
}

void handlePutItem() {
  if (!server.hasArg("id")) {
    updateDisplay("Erro: ID faltando");
    server.send(400, "text/plain", "Parâmetro id faltando");
    return;
  }
  
  int id = server.arg("id").toInt();
  
  if (id <= 0 || id > itemCount) {
    updateDisplay("Erro: item nao encontrado");
    server.send(404, "text/plain", "Item não encontrado");
    return;
  }
  
  if (!server.hasArg("plain")) {
    updateDisplay("Erro: corpo vazio");
    server.send(400, "text/plain", "Corpo da requisição vazio");
    return;
  }
  
  String body = server.arg("plain");
  
  // Parse manual do JSON simples
  int nameStart = body.indexOf("\"name\":\"") + 8;
  int nameEnd = body.indexOf("\"", nameStart);
  int descStart = body.indexOf("\"text\":\"") + 15;
  int descEnd = body.indexOf("\"", descStart);
  
  if (nameStart < 8 || nameEnd == -1 || descStart < 15 || descEnd == -1) {
    updateDisplay("Erro: JSON invalido");
    server.send(400, "text/plain", "Formato JSON inválido");
    return;
  }
  
  String oldName = items[id-1].name;
  items[id-1].name = body.substring(nameStart, nameEnd);
  items[id-1].text = body.substring(descStart, descEnd);
  
  updateDisplay("Atualizado: " + oldName);
  server.send(200, "application/json", "{\"status\":\"updated\"}");
}

void handleDeleteItem() {
  if (!server.hasArg("id")) {
    updateDisplay("Erro: ID faltando");
    server.send(400, "text/plain", "Parâmetro id faltando");
    return;
  }
  
  int id = server.arg("id").toInt();
  
  if (id <= 0 || id > itemCount) {
    updateDisplay("Erro: item nao encontrado");
    server.send(404, "text/plain", "Item não encontrado");
    return;
  }
  
  String deletedName = items[id-1].name;
  
  // Remove o item movendo os itens seguintes
  for (int i = id-1; i < itemCount-1; i++) {
    items[i] = items[i+1];
    items[i].id = i+1;
  }
  itemCount--;
  
  updateDisplay("Removido: " + deletedName);
  server.send(200, "application/json", "{\"status\":\"deleted\"}");
}

void handleNotFound() {
  updateDisplay("Rota nao encontrada");
  String message = "Endpoint não encontrado\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}