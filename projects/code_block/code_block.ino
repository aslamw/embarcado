#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "esp_wifi.h"

// Configurações
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SDA_PIN 8
#define SCL_PIN 9
#define PinLed 24
#define Motor 14

const char* softAP_ssid = "ESP32-S3_Config";
const char* softAP_password = "123456789";

struct Block {
  String type;
  String value;
};
Block blocks[10];
int blockCount = 0;
bool programRunning = false;
bool shouldReset = false;
bool shouldStop =false;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer webServer(80);
DNSServer dnsServer;

// Protótipos de funções
void setupWiFiAP();
String generateBlocksHTML();
void handleRoot();
void handleExecute();
void handleRemove();
void handleReset();

void setupWiFiAP() {
  WiFi.softAP(softAP_ssid, softAP_password);
  WiFi.softAPsetHostname("esp32-control");
  
  // Configuração WiFi
  wifi_config_t wifi_config;
  esp_wifi_get_config(WIFI_IF_AP, &wifi_config);
  wifi_config.ap.authmode = WIFI_AUTH_OPEN;
  wifi_config.ap.channel = 1;
  wifi_config.ap.max_connection = 4;
  esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
  
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

String generateBlocksHTML() {
  if (blockCount == 0) {
    return "<p style='text-align: center; color: #666;'>Nenhum bloco adicionado</p>";
  }
  
  String html = "";
  for (int i = 0; i < blockCount; i++) {
    html += "<div class='block' data-type='" + blocks[i].type + "' id='block-" + String(i) + "'>";
    html += "<span class='emoji'>";
    html += (blocks[i].type == "led") ? "💡" : (blocks[i].type == "motor") ? "⚙️" : "⏱️";
    html += "</span> ";
    html += blocks[i].type + ": " + blocks[i].value;
    html += "<button class='remove-btn' onclick='removeBlock(" + String(i) + ")'>×</button>";
    html += "</div>";
  }
  return html;
}

void handleRoot() {
  String page = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Controle de Blocos ESP32</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    /* Estilos mantidos iguais */
    .running { background: #4CAF50 !important; }
    #reset-btn { background: #f44336; margin-left: 10px; }
  </style>
</head>
<body>
  <!-- Cabeçalho e paleta mantidos iguais -->
  
  <div style="display: flex; justify-content: center;">
    <button id="run-btn" onclick="executeProgram()">Executar no ESP32</button>
    <button id="reset-btn" onclick="resetProgram()">Resetar</button>
  </div>

  <script>
    function removeBlock(index) {
      fetch('/remove?index=' + index, { method: 'POST' })
        .then(() => location.reload());
    }
    
    function executeProgram() {
     // Modificar a função de clique do botão "Brincar"
  document.getElementById('run-btn').addEventListener('click', async function() {
  const btn = this;
  const blocks = document.querySelectorAll('#workspace .block');
  
  if (blocks.length === 0) {
    alert('Adicione blocos primeiro!');
    return;
  }
  
  btn.disabled = true;
  btn.textContent = 'Executando...';
  document.getElementById('status').innerHTML = '😊 Executando programa...';
  
  try {
    const formData = new FormData();
    blocks.forEach((block, index) => {
      formData.append(`block_${index}_type`, block.dataset.type);
      formData.append(`block_${index}_value`, block.dataset.value);
    });
    
    const response = await fetch('/execute', {
      method: 'POST',
      body: formData
    });
    
    if (response.ok) {
      const result = await response.text();
      console.log(result);
      btn.disabled = false;
      btn.textContent = '🎮 Brincar com o ESP32 Amigo!';
      document.getElementById('status').innerHTML = '😊 Execução concluída!';
    }
  } catch (error) {
    console.error('Erro:', error);
    btn.disabled = false;
    btn.textContent = '🎮 Brincar com o ESP32 Amigo!';
    document.getElementById('status').innerHTML = '😢 Erro na execução';
  }
});
    }
    
    function resetProgram() {
      fetch('/reset', { method: 'POST' })
        .then(() => location.reload());
    }
  </script>
</body>
</html>
)=====";

  page.replace("%BLOCKS%", generateBlocksHTML());
  webServer.send(200, "text/html", page);
}

void handleExecute() {
  programRunning = true;
  shouldStop = false;
  
  // Limpar blocos antigos e carregar novos
  blockCount = 0;
  for (int i = 0; i < 10; i++) {
    String type = webServer.arg("block_" + String(i) + "_type");
    String value = webServer.arg("block_" + String(i) + "_value");
    
    if (type == "") break;
    
    blocks[blockCount].type = type;
    blocks[blockCount].value = value;
    blockCount++;
    
    Serial.printf("Bloco %d: %s -> %s\n", i, type.c_str(), value.c_str());
  }

  // Executar apenas UMA VEZ a sequência de blocos
  for (int i = 0; i < blockCount && !shouldStop; i++) {
    if (blocks[i].type == "led") {
      bool state = (blocks[i].value.indexOf("acender") != -1);
      digitalWrite(PinLed, state ? HIGH : LOW);
      Serial.println(state ? "LED LIGADO" : "LED DESLIGADO");
    } 
    else if (blocks[i].type == "motor") {
      bool state = (blocks[i].value.indexOf("ligar") != -1);
      digitalWrite(Motor, state ? HIGH : LOW);
      Serial.println(state ? "MOTOR LIGADO" : "MOTOR DESLIGADO");
    }
    else if (blocks[i].type == "repeat") {
      int delayTime = blocks[i].value.toInt();
      if (delayTime <= 0) delayTime = 1000;
      Serial.printf("PAUSA: %dms\n", delayTime);
      
      // Pausa fracionada para permitir parada
      for (int j = 0; j < delayTime/100 && !shouldStop; j++) {
        delay(100);
      }
    }
  }
  
  programRunning = false;
  webServer.send(200, "text/plain", "Execução concluída");
}

void handleRemove() {
  int index = webServer.arg("index").toInt();
  if (index >= 0 && index < blockCount) {
    for (int i = index; i < blockCount-1; i++) {
      blocks[i] = blocks[i+1];
    }
    blockCount--;
  }
  handleRoot();
}

void handleReset() {
  shouldReset = true;
  blockCount = 0;
  digitalWrite(PinLed, LOW);
  digitalWrite(Motor, LOW);
  delay(100); // Tempo para parar a execução atual
  shouldReset = false;
  handleRoot();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha no display OLED"));
    for (;;);
  }

  pinMode(PinLed, OUTPUT);
  pinMode(Motor, OUTPUT);
  
  setupWiFiAP();
  dnsServer.start(53, "*", WiFi.softAPIP());
  
  webServer.on("/", handleRoot);
  webServer.on("/execute", HTTP_POST, handleExecute);
  webServer.on("/remove", HTTP_POST, handleRemove);
  webServer.on("/reset", HTTP_POST, handleReset);
  webServer.begin();
  
  Serial.println("Servidor iniciado");
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(programRunning ? "Executando..." : "Pronto");
  
  for (int i = 0; i < blockCount && i < 3; i++) {
    display.println(blocks[i].type + ": " + blocks[i].value);
  }
  
  display.display();
  delay(100);
}