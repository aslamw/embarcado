#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

// =============================================
// CONFIGURAÇÃO DE PINOS PARA ESP32 V1
// =============================================
#define LED_BUILTIN 2     // LED integrado na maioria das placas ESP32 V1
#define MOTOR_PIN 23      // Pino para o motor/relé
#define SDA_PIN 21        // Pinos I2C padrão
#define SCL_PIN 22

const char* boardName = "ESP32 V1";

// =============================================
// CONFIGURAÇÕES GERAIS
// =============================================
const char* apSSID = "ESP32_Control";
const char* apPassword = "12345678"; // Mínimo 8 caracteres

// Variáveis controláveis via web
int var1 = 0, var2 = 0, var3 = 0;
String str1 = "Padrao1", str2 = "Padrao2";

WebServer server(80);
DNSServer dnsServer;

// =============================================
// PÁGINA HTML
// =============================================
const char* htmlPage = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Controle ESP32</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { 
      font-family: Arial, sans-serif; 
      max-width: 500px; 
      margin: 0 auto; 
      padding: 20px;
      background-color: #f5f5f5;
    }
    h1 {
      color: #2c3e50;
      text-align: center;
    }
    .control-panel {
      background: white;
      padding: 20px;
      border-radius: 8px;
      box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    }
    .input-group {
      margin-bottom: 15px;
    }
    label {
      display: block;
      margin-bottom: 5px;
      font-weight: bold;
      color: #34495e;
    }
    input {
      width: 100%;
      padding: 10px;
      border: 1px solid #ddd;
      border-radius: 4px;
      box-sizing: border-box;
    }
    button {
      width: 100%;
      padding: 12px;
      background-color: #3498db;
      color: white;
      border: none;
      border-radius: 4px;
      font-size: 16px;
      cursor: pointer;
      transition: background-color 0.3s;
    }
    button:hover {
      background-color: #2980b9;
    }
    .status {
      margin-top: 20px;
      padding: 15px;
      background: #ecf0f1;
      border-radius: 4px;
      font-size: 14px;
    }
  </style>
</head>
<body>
  <div class="control-panel">
    <h1>Controle ESP32 V1</h1>
    <form action="/update" method="post">
      <div class="input-group">
        <label>Variável 1 (Controla LED):</label>
        <input type="number" name="var1" value="%VAR1%" min="0" max="1">
      </div>
      <div class="input-group">
        <label>Variável 2 (Controla Motor):</label>
        <input type="number" name="var2" value="%VAR2%" min="0" max="1">
      </div>
      <div class="input-group">
        <label>Variável 3 (Inteira):</label>
        <input type="number" name="var3" value="%VAR3%">
      </div>
      <div class="input-group">
        <label>String 1:</label>
        <input type="text" name="str1" value="%STR1%">
      </div>
      <div class="input-group">
        <label>String 2:</label>
        <input type="text" name="str2" value="%STR2%">
      </div>
      <button type="submit">Atualizar Valores</button>
    </form>
    <div class="status">
      <p><strong>Placa:</strong> %BOARDNAME%</p>
      <p><strong>Endereço IP:</strong> %IP%</p>
      <p><strong>Status:</strong> %STATUS%</p>
      <p><strong>LED:</strong> %LEDSTATUS%</p>
      <p><strong>Motor:</strong> %MOTORSTATUS%</p>
    </div>
  </div>
</body>
</html>
)rawliteral";

// =============================================
// FUNÇÕES AUXILIARES
// =============================================
String getStatusHTML() {
  String page = htmlPage;
  page.replace("%BOARDNAME%", boardName);
  page.replace("%VAR1%", String(var1));
  page.replace("%VAR2%", String(var2));
  page.replace("%VAR3%", String(var3));
  page.replace("%STR1%", str1);
  page.replace("%STR2%", str2);
  page.replace("%IP%", WiFi.softAPIP().toString());
  page.replace("%STATUS%", "Conectado");
  page.replace("%LEDSTATUS%", var1 == 1 ? "Ligado" : "Desligado");
  page.replace("%MOTORSTATUS%", var2 == 1 ? "Ligado" : "Desligado");
  return page;
}

void updateOutputs() {
  digitalWrite(LED_BUILTIN, var1 == 1 ? HIGH : LOW);
  digitalWrite(MOTOR_PIN, var2 == 1 ? HIGH : LOW);
}

void printStatus() {
  Serial.println("\n=== Status Atual ===");
  Serial.println("Placa: " + String(boardName));
  Serial.println("IP: " + WiFi.softAPIP().toString());
  Serial.println("var1 (LED): " + String(var1));
  Serial.println("var2 (Motor): " + String(var2));
  Serial.println("var3: " + String(var3));
  Serial.println("str1: " + str1);
  Serial.println("str2: " + str2);
  Serial.println("===================");
}

// =============================================
// HANDLERS DO SERVIDOR WEB
// =============================================
void handleRoot() {
  server.send(200, "text/html", getStatusHTML());
}

void handleUpdate() {
  // Atualiza variáveis
  if (server.hasArg("var1")) var1 = server.arg("var1").toInt();
  if (server.hasArg("var2")) var2 = server.arg("var2").toInt();
  if (server.hasArg("var3")) var3 = server.arg("var3").toInt();
  if (server.hasArg("str1")) str1 = server.arg("str1");
  if (server.hasArg("str2")) str2 = server.arg("str2");
  
  // Atualiza saídas físicas
  updateOutputs();
  
  // Log e resposta
  printStatus();
  handleRoot();
}

void handleNotFound() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "Redirecionando para a página principal...");
}

// =============================================
// CONFIGURAÇÃO WIFI
// =============================================
void setupWiFiAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);
  
  // Configuração do DNS para captive portal
  dnsServer.start(53, "*", WiFi.softAPIP());
  
  Serial.println("\nPonto de Acesso WiFi Configurado:");
  Serial.println("SSID: " + String(apSSID));
  Serial.println("Senha: " + String(apPassword));
  Serial.println("IP: " + WiFi.softAPIP().toString());
}

// =============================================
// SETUP E LOOP PRINCIPAIS
// =============================================
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Configura pinos
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(MOTOR_PIN, LOW);
  
  // Inicia WiFi
  setupWiFiAP();
  
  // Configura rotas do servidor
  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.onNotFound(handleNotFound);
  
  server.begin();
  
  Serial.println("\nServidor HTTP iniciado");
  printStatus();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  
  // Pisca LED rapidamente se var1 for 0 (modo de espera)
  if(var1 == 0) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);
  }
}