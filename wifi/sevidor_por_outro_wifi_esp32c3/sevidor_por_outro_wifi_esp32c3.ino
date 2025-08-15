#include <WiFi.h>
#include <Wire.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SDA_PIN 6
#define SCL_PIN 7

#define led_pin 8

// Substitua com suas credenciais de rede
const char* ssid = "atom";
const char* password = "neide94177705";

WebServer server(80); // Servidor na porta 80

// Estrutura de dados simples para exemplo
String items[10];
int itemCount = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void print_text(char text[100], int X, int Y){

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(X,Y);
  display.print(text);
  display.display();
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(led_pin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  // Conectar ao WiFi
  WiFi.begin(ssid, password);
  print_text("Conectando ao WiFi",10,10);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
    // Mostrar IP no display
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Conectado!");
  display.println(WiFi.SSID());
  display.println("IP:");
  display.println(WiFi.localIP());
  display.display();
  
  Serial.println("");
  Serial.println("Conectado ao WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Configurar rotas do servidor
  server.on("/", HTTP_GET, handleRoot);
  server.on("/items", HTTP_GET, handleGetItems);
  server.on("/items", HTTP_POST, handlePostItem);
  server.on("/items", HTTP_PUT, handlePutItem);
  server.on("/items", HTTP_DELETE, handleDeleteItem);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

// Função para enviar resposta JSON
void sendJsonResponse(int statusCode, String message, String data = "") {
  String response = "{\"status\": \"" + message + "\"";
  if (data != "") {
    response += ", \"data\": " + data;
  }
  response += "}";
  
  server.send(statusCode, "application/json", response);
}

// Handlers das rotas
void handleRoot() {
  String html = "<h1>Servidor CRUD ESP32-C3</h1>";
  html += "<p>Endpoints disponíveis:</p>";
  html += "<ul>";
  html += "<li>GET /items - Listar itens</li>";
  html += "<li>POST /items - Adicionar item</li>";
  html += "<li>PUT /items - Atualizar item</li>";
  html += "<li>DELETE /items - Remover item</li>";
  html += "</ul>";
  
  server.send(200, "text/html", html);
}

void handleGetItems() {
  String json = "[";
  for (int i = 0; i < itemCount; i++) {
    json += "\"" + items[i] + "\"";
    if (i < itemCount - 1) json += ",";
  }
  json += "]";
  
  sendJsonResponse(200, "success", json);
}

void handlePostItem() {
  if (server.hasArg("plain")) {
    String newItem = server.arg("plain");
    
    // Verifica se é um JSON válido (simplificado)
    if (newItem.indexOf('{') != -1 && newItem.indexOf('}') != -1) {
      if (itemCount < 10) {
        items[itemCount] = newItem;
        itemCount++;
        sendJsonResponse(201, "Item adicionado com sucesso");
      } else {
        sendJsonResponse(400, "Limite de itens atingido");
      }
    } else {
      sendJsonResponse(400, "JSON inválido");
    }
  } else {
    sendJsonResponse(400, "Dados não fornecidos");
  }
}

void handlePutItem() {
  if (server.hasArg("id") && server.hasArg("plain")) {
    int id = server.arg("id").toInt();
    String updatedItem = server.arg("plain");
    
    if (id >= 0 && id < itemCount) {
      items[id] = updatedItem;
      sendJsonResponse(200, "Item atualizado");
    } else {
      sendJsonResponse(404, "Item não encontrado");
    }
  } else {
    sendJsonResponse(400, "Parâmetros inválidos");
  }
}

void handleDeleteItem() {
  if (server.hasArg("id")) {
    int id = server.arg("id").toInt();
    
    if (id >= 0 && id < itemCount) {
      // Remover item deslocando os demais
      for (int i = id; i < itemCount - 1; i++) {
        items[i] = items[i + 1];
      }
      itemCount--;
      sendJsonResponse(200, "Item removido");
    } else {
      sendJsonResponse(404, "Item não encontrado");
    }
  } else {
    sendJsonResponse(400, "ID não fornecido");
  }
}