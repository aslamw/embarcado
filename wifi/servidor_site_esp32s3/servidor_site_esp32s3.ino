#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdbool.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SDA_PIN 8
#define SCL_PIN 9

#define PinLed 2
#define Motor 14

// Configurações do Ponto de Acesso
const char* softAP_ssid = "ESP32-S3_Config";  // Nome da rede visível
const char* softAP_password = "123456789";           // Rede aberta (sem senha)

// Variáveis controláveis
int var1 = 0, var2 = 0, var3 = 0;
String str1 = "Padrão1", str2 = "Padrão2";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WebServer webServer(80);
DNSServer dnsServer;

// Página HTML otimizada
const char* captivePage = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Controle ESP32-S3</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="0; url=http://192.168.4.1/" />
  <style>
    body {font-family: Arial, sans-serif; text-align: center; padding: 20px;}
    .container {max-width: 500px; margin: 0 auto;}
    input, button {padding: 10px; margin: 5px; width: 95%;}
    button {background-color: #4CAF50; color: white; border: none;}
  </style>
</head>
<body>
  <div class="container">
    <h2>Redirecionando para o painel de controle...</h2>
    <p>Se não for redirecionado automaticamente, <a href="http://192.168.4.1/">clique aqui</a></p>
  </div>
</body>
</html>
)=====";

// Página principal de controle
const String controlPage = FPSTR(R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Controle ESP32-S3</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {font-family: Arial, sans-serif; padding: 20px;}
    .container {max-width: 500px; margin: 0 auto;}
    .input-group {margin-bottom: 15px;}
    input, button {padding: 10px; width: 100%; box-sizing: border-box;}
    button {background-color: #4CAF50; color: white; border: none; margin-top: 10px;}
  </style>
</head>
<body>
  <div class="container">
    <h1>Controle de Variáveis</h1>
    <form action="/update" method="post">
      <div class="input-group">
        <label>Variável 1 (Inteira):</label>
        <input type="number" name="var1" value="%VAR1%">
      </div>
      <div class="input-group">
        <label>Variável 2 (Inteira):</label>
        <input type="number" name="var2" value="%VAR2%">
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
    <p>%STATUS%</p>
  </div>
</body>
</html>
)=====");

void setupWiFiAP() {
  WiFi.softAP(softAP_ssid, softAP_password);
  WiFi.softAPsetHostname("esp32-control");
  
  // Otimização para melhor desempenho do AP
  esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G);
  esp_wifi_set_bandwidth(WIFI_IF_AP, WIFI_BW_HT20);
  
  Serial.println();
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void handleRoot() {
  String page = controlPage;
  page.replace("%VAR1%", String(var1));
  page.replace("%VAR2%", String(var2));
  page.replace("%VAR3%", String(var3));
  page.replace("%STR1%", str1);
  page.replace("%STR2%", str2);
  page.replace("%STATUS%", "Conectado");
  webServer.send(200, "text/html", page);
}

void handleUpdate() {
  if (webServer.hasArg("var1")) var1 = webServer.arg("var1").toInt();
  if (webServer.hasArg("var2")) var2 = webServer.arg("var2").toInt();
  if (webServer.hasArg("var3")) var3 = webServer.arg("var3").toInt();
  if (webServer.hasArg("str1")) str1 = webServer.arg("str1");
  if (webServer.hasArg("str2")) str2 = webServer.arg("str2");
  
  handleRoot(); // Volta para a página principal com valores atualizados
  Serial.printf("Valores atualizados: %d, %d, %d, %s, %s\n", var1, var2, var3, str1.c_str(), str2.c_str());
}

void handleCaptivePortal() {
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
  webServer.send(200, "text/html", captivePage);
}

void print_text(char text[100], int X, int Y){

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(X,Y);
  display.println(text);
  display.display();
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();

  setupWiFiAP();

  pinMode(PinLed, OUTPUT);
  pinMode(Motor, OUTPUT);
  
  // Inicia servidor DNS para captive portal
  dnsServer.start(53, "*", WiFi.softAPIP());
  
  // Configura rotas do servidor web
  webServer.on("/", handleRoot);
  webServer.on("/update", handleUpdate);
  webServer.on("/generate_204", handleCaptivePortal);  // Android
  webServer.on("/fwlink", handleCaptivePortal);        // Windows
  webServer.onNotFound(handleCaptivePortal);           // Todas outras URLs
  
  webServer.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  //digitalWrite(PinLed,  (var1 == 1) ? HIGH : LOW);
  digitalWrite(PinLed,HIGH);
  digitalWrite(Motor,  (var2 == 1) ? HIGH : LOW);

  
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(str1);
  display.println(str2);
  display.display();
  delay(1000);
}