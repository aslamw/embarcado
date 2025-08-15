#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SDA_PIN 6
#define SCL_PIN 7

// Configurações de WiFi
const char* ssid = "esp32_s3";
const char* password = "12345678";

// Endereço do servidor local
const char* serverUrl = "http://192.168.4.1"; //Porta do seu servidor
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  delay(1000);

  // Conectar ao WiFi
  WiFi.begin(ssid, password);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  print_text("Conectando ao WiFi...",0,0);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);

  display.println("");
  display.println("Conectado ao WiFi");
  display.print("Endereço IP: ");
  display.println(WiFi.localIP());

  display.display();
  delay(1000);

  // Testar operações CRUD
  //testCRUDOperations();
}

void loop() {
  readItems();
  delay(1000);
}

void print_text(char text[100], int X, int Y){

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(X,Y);
  display.print(text);
  display.display();
  delay(1000);
}

void testCRUDOperations() {
  // CREATE - Adicionar um novo item
  createItem("{\"name\":\"Item1\",\"value\":10}");

  // READ - Ler todos os itens
  readItems();

  // UPDATE - Atualizar um item (assumindo que o ID é 1)
  updateItem(1, "{\"name\":\"Item1Atualizado\",\"value\":20}");

  // DELETE - Remover um item (assumindo que o ID é 1)
  deleteItem(1);

  // Ler novamente para verificar as mudanças
  readItems();
}

void createItem(String payload) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    
    http.begin(String(serverUrl) + "/items"); // Altere o endpoint conforme necessário
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("CREATE Response: " + response);
    } else {
      display.println("Erro no CREATE: ");
      display.println(httpResponseCode);
    }
    
    http.end();
  } else {
    display.print("WiFi Desconectado");
  }

  display.display();
  delay(1000);
}

void readItems() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    http.begin(String(serverUrl) + "/items");

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      display.println("READ Response: " + response);
    } else {
      display.println("Erro no READ: ");
      display.println(httpResponseCode);
    }
    
    http.end();
  } else {
    display.println("WiFi Desconectado");
  }
  display.display();
  delay(1000);
}

void updateItem(int id, String payload) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    http.begin(String(serverUrl) + "/items/" + String(id)); // Altere o endpoint conforme necessário
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.PUT(payload);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("UPDATE Response: " + response);
    } else {
      Serial.print("Erro no UPDATE: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi Desconectado");
  }
}

void deleteItem(int id) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    http.begin(String(serverUrl) + "/items/" + String(id)); // Altere o endpoint conforme necessário
    
    int httpResponseCode = http.sendRequest("DELETE");
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("DELETE Response: " + response);
    } else {
      Serial.print("Erro no DELETE: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  } else {
    Serial.println("WiFi Desconectado");
  }
}