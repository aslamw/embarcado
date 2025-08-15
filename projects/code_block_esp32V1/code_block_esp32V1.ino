#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configurações de hardware para ESP32 v1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define SDA_PIN 21  // Pino padrão SDA para ESP32 v1
#define SCL_PIN 22  // Pino padrão SCL para ESP32 v1
#define PinLed 2    // Usando o LED onboard (GPIO2)
#define Motor 19     // GPIO4 para o motor

// Configurações do WiFi
const char* softAP_ssid = "ESP32_Config";
const char* softAP_password = "123456789";

// Estrutura para armazenar os blocos
struct Block {
  String type;
  String value;
};
Block blocks[10];
int blockCount = 0;
bool shouldStop = false;
unsigned long blockStartTime = 0;
int currentBlockIndex = -1;  // -1 significa que nenhum bloco está sendo executado
bool isExecuting = false;
bool shouldRepeat = true;
void executeCurrentBlock();
void updateDisplay();
void stopExecution();

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer webServer(80);
DNSServer dnsServer;

void setupWiFiAP() {
  WiFi.softAP(softAP_ssid, softAP_password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

String generateBlocksHTML() {
  if (blockCount == 0) {
    return "<div class='empty-workspace'><p>Arraste os blocos para cá!</p><img src='data:image/svg+xml;utf8,<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100\" height=\"100\" viewBox=\"0 0 100 100\"><circle cx=\"50\" cy=\"50\" r=\"40\" fill=\"%23FFD166\" stroke=\"%23FF9F1C\" stroke-width=\"2\"/><text x=\"50\" y=\"55\" font-family=\"Comic Sans MS\" font-size=\"14\" text-anchor=\"middle\" fill=\"%236B2D42\">Solte aqui!</text></svg>'></div>";
  }
  
  String html = "";
  for (int i = 0; i < blockCount; i++) {
    html += "<div class='block' data-type='" + blocks[i].type + "' id='block-" + String(i) + "'>";
    html += "<span class='object-icon'>";
    html += (blocks[i].type == "led") ? "💡" : (blocks[i].type == "motor") ? "⚙️" : "⏱️";
    html += "</span> " + blocks[i].type + ": " + blocks[i].value;
    html += "<button class='remove-btn' onclick='removeBlock(" + String(i) + ")'>❌</button>";
    html += "</div>";
  }
  return html;
}

void handleRoot() {
  String page = R"=====(
<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Brincando de Programar - ESP32 Amigo</title>
  <style>
    body {
      font-family: 'Comic Sans MS', cursive;
      background-color: #f0f8ff;
      margin: 0;
      padding: 10px;
    }
    .header {
      background-color: #FF6B6B;
      color: white;
      padding: 12px;
      text-align: center;
      border-radius: 15px;
      box-shadow: 0 4px 8px rgba(0,0,0,0.1);
      margin-bottom: 10px;
    }
    .container {
      display: flex;
      flex-direction: column;
      min-height: 80vh;
    }
    @media (min-width: 768px) {
      .container {
        flex-direction: row;
      }
    }
    .block-palette {
      width: 100%;
      background-color: #FFD166;
      padding: 12px;
      border-radius: 15px;
      border: 3px dotted #FF9F1C;
      margin-bottom: 15px;
    }
    @media (min-width: 768px) {
      .block-palette {
        width: 30%;
        margin-bottom: 0;
        border-radius: 15px 0 0 15px;
      }
    }
    .workspace {
      width: 100%;
      background-color: #F7FFF7;
      padding: 15px;
      min-height: 300px;
      border-radius: 15px;
      border: 3px dotted #4ECDC4;
      position: relative;
      background-image: url('data:image/svg+xml;utf8,<svg width="100" height="100" viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg"><circle cx="50" cy="50" r="40" fill="none" stroke="%234ECDC4" stroke-width="0.5"/></svg>');
      background-size: 50px 50px;
    }
    @media (min-width: 768px) {
      .workspace {
        width: 70%;
        border-radius: 0 15px 15px 0;
      }
    }
    .block {
      background-color: #FF9F1C;
      padding: 12px;
      margin: 12px 0;
      border-radius: 10px;
      cursor: grab;
      text-align: center;
      box-shadow: 4px 4px 0px rgba(0,0,0,0.1);
      font-size: 16px;
      border: 2px solid #FF6B6B;
      transition: transform 0.2s;
      position: relative;
    }
    .block:active {
      cursor: grabbing;
      transform: scale(1.02);
    }
    .block.green {
      background-color: #06D6A0;
      color: white;
      border-color: #1A936F;
    }
    .block.purple {
      background-color: #A663CC;
      color: white;
      border-color: #6A2C70;
    }
    .block.red {
      background-color: #EF476F;
      color: white;
      border-color: #B23A48;
    }
    #run-btn {
      background-color: #FF6B6B;
      color: white;
      border: none;
      padding: 12px 24px;
      font-size: 18px;
      border-radius: 50px;
      cursor: pointer;
      margin: 15px auto;
      display: block;
      box-shadow: 0 4px 8px rgba(0,0,0,0.2);
      transition: all 0.3s;
      width: 90%;
      max-width: 300px;
    }
    #run-btn:hover {
      transform: scale(1.05);
      background-color: #EF476F;
    }
    #stop-btn {
      background-color: #EF476F;
      color: white;
      border: none;
      padding: 12px 24px;
      font-size: 18px;
      border-radius: 50px;
      cursor: pointer;
      margin: 15px auto;
      display: none;
      box-shadow: 0 4px 8px rgba(0,0,0,0.2);
      transition: all 0.3s;
      width: 90%;
      max-width: 300px;
    }
    #stop-btn:hover {
      transform: scale(1.05);
      background-color: #B23A48;
    }
    #reset-btn {
      background-color: #A663CC;
      color: white;
      border: none;
      padding: 12px 24px;
      font-size: 18px;
      border-radius: 50px;
      cursor: pointer;
      margin: 15px auto;
      display: block;
      box-shadow: 0 4px 8px rgba(0,0,0,0.2);
      transition: all 0.3s;
      width: 90%;
      max-width: 300px;
    }
    #reset-btn:hover {
      transform: scale(1.05);
      background-color: #6A2C70;
    }
    .device-status {
      background-color: #CBF3F0;
      padding: 15px;
      border-radius: 15px;
      margin-top: 15px;
      border: 3px dashed #2EC4B6;
    }
    .emoji {
      font-size: 24px;
      vertical-align: middle;
    }
    h2 {
      color: #2B2D42;
      border-bottom: 3px solid #FF9F1C;
      padding-bottom: 8px;
      font-size: 1.3rem;
    }
    select {
      padding: 6px;
      border-radius: 8px;
      border: 2px solid #4ECDC4;
      font-family: 'Comic Sans MS', cursive;
      margin: 5px 0;
      width: 100%;
      max-width: 200px;
    }
    .object-icon {
      font-size: 30px;
      margin-right: 8px;
      vertical-align: middle;
    }
    .welcome-message {
      background-color: #FFD166;
      padding: 12px;
      border-radius: 10px;
      margin-bottom: 15px;
      text-align: center;
      font-size: 16px;
    }
    .empty-workspace {
      text-align: center;
      padding: 20px;
      color: #6B2D42;
    }
    .empty-workspace img {
      width: 80px;
      margin: 10px auto;
      display: block;
    }
    .remove-btn {
      position: absolute;
      right: 10px;
      top: 50%;
      transform: translateY(-50%);
      background: none;
      border: none;
      font-size: 18px;
      cursor: pointer;
      color: white;
    }
    .btn-container {
      display: flex;
      gap: 10px;
      justify-content: center;
      flex-wrap: wrap;
    }
  </style>
</head>
<body>
  <div class="header">
    <h1>🐱 Brincando de Programar com o ESP32 Amigo 🐶</h1>
  </div>
  
  <div class="welcome-message">
    Olá, pequeno programador! 👋<br>
    Arraste os blocos para montar seu programa!
  </div>
  
  <div class="container">
    <div class="block-palette">
      <h2>📦 Caixa de Blocos Mágicos</h2>
      
      <div class="block green" draggable="true" data-type="led">
        <span class="object-icon">💡</span> Quando a Luz:
        <select>
          <option>acender 🔦</option>
          <option>apagar 🌙</option>
        </select>
      </div>
      
      <div class="block purple" draggable="true" data-type="motor">
        <span class="object-icon">⚙️</span> Quando o Motor:
        <select>
          <option>ligar 🚀</option>
          <option>desligar 🛑</option>
        </select>
      </div>
      
      <div class="block red" draggable="true" data-type="repeat">
        <span class="object-icon">⏱️</span> Pausa: 1s
      </div>
    </div>
    
    <div class="workspace" id="workspace">
      <h2>🌟 Sua Programação Mágica</h2>
      %BLOCKS%
    </div>
  </div>

  <div class="btn-container">
    <button id="run-btn">🎮 Brincar com o ESP32 Amigo!</button>
    <button id="stop-btn">⏹️ Parar Execução</button>
    <button id="reset-btn">🔄 Reiniciar Tudo</button>
  </div>
  
  <div class="device-status">
    <h3>📱 Como está seu ESP32 Amigo?</h3>
    <p id="status">😴 Dormindo (não conectado ainda)</p>
    <p>🌐 Endereço: <span id="ip">%IP%</span></p>
  </div>

  <script>
    document.addEventListener('DOMContentLoaded', function() {
      const workspace = document.getElementById('workspace');
      const emptyWorkspace = workspace.querySelector('.empty-workspace');
      const runBtn = document.getElementById('run-btn');
      const stopBtn = document.getElementById('stop-btn');
      const resetBtn = document.getElementById('reset-btn');
      const status = document.getElementById('status');
      const ip = document.getElementById('ip');
      
      // Atualizar IP
      ip.textContent = window.location.hostname || '192.168.4.1';
      
      // Permitir que blocos sejam arrastados
      document.querySelectorAll('.block-palette .block').forEach(block => {
        block.addEventListener('dragstart', function(e) {
          const select = this.querySelector('select');
          const value = select ? select.value : '1s';
          e.dataTransfer.setData('text/plain', this.dataset.type + ':' + value);
        });
      });
      
      // Permitir soltar blocos na área de trabalho
      workspace.addEventListener('dragover', function(e) {
        e.preventDefault();
        e.dataTransfer.dropEffect = 'copy';
      });
      
      workspace.addEventListener('drop', function(e) {
        e.preventDefault();
        const [type, value] = e.dataTransfer.getData('text/plain').split(':');
        
        const newBlock = document.createElement('div');
        newBlock.className = 'block ' + type;
        newBlock.dataset.type = type;
        newBlock.dataset.value = value;
        
        let icon, content;
        if (type === 'led') {
          icon = '💡';
          content = `LED: ${value}`;
        } else if (type === 'motor') {
          icon = '⚙️';
          content = `Motor: ${value}`;
        } else {
          icon = '⏱️';
          content = `Pausa: ${value}`;
        }
        
        newBlock.innerHTML = `
          <span class="object-icon">${icon}</span> ${content}
          <button class="remove-btn" onclick="removeBlock(this.parentElement)">❌</button>
        `;
        
        workspace.appendChild(newBlock);
        
        // Esconder mensagem inicial
        if (emptyWorkspace) {
          emptyWorkspace.style.display = 'none';
        }
      });
      
      // Executar programa
      runBtn.addEventListener('click', async function() {
        const blocks = workspace.querySelectorAll('.block');
        if (blocks.length === 0) {
          alert('Adicione blocos primeiro!');
          return;
        }
        
        runBtn.disabled = true;
        runBtn.textContent = 'Executando...';
        stopBtn.style.display = 'block';
        status.innerHTML = '😊 Executando programa...';
        
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
            runBtn.disabled = false;
            runBtn.textContent = '🎮 Brincar com o ESP32 Amigo!';
            //stopBtn.style.display = 'none';
            status.innerHTML = '😊 Pronto para executar';
          }
        } catch (error) {
          console.error('Erro:', error);
          runBtn.disabled = false;
          runBtn.textContent = '🎮 Brincar com o ESP32 Amigo!';
          //stopBtn.style.display = 'none';
          status.innerHTML = '😢 Erro na execução';
        }
      });
      
      // Parar execução
      stopBtn.addEventListener('click', async function() {
        //stopBtn.disabled = true;
        stopBtn.textContent = 'Parando...';
        status.innerHTML = '🛑 Parando execução...';
        
        try {
          const response = await fetch('/stop', { method: 'POST' });
          if (response.ok) {
            runBtn.disabled = false;
            runBtn.textContent = '🎮 Brincar com o ESP32 Amigo!';
            stopBtn.style.display = 'none';
            stopBtn.disabled = false;
            stopBtn.textContent = '⏹️ Parar Execução';
            status.innerHTML = '😊 Execução parada';
          }
        } catch (error) {
          console.error('Erro ao parar:', error);
          stopBtn.disabled = false;
          stopBtn.textContent = '⏹️ Parar Execução';
          status.innerHTML = '😢 Erro ao parar';
        }
      });
      
      // Reiniciar tudo
      resetBtn.addEventListener('click', async function() {
        const response = await fetch('/reset', { method: 'POST' });
        if (response.ok) {
          location.reload();
        }
      });
    });
    
    function removeBlock(blockElement) {
      blockElement.style.transform = 'scale(0)';
      setTimeout(() => {
        blockElement.remove();
        const workspace = document.getElementById('workspace');
        if (workspace.children.length === 2) { // Se só sobrou o h2
          const emptyDiv = document.createElement('div');
          emptyDiv.className = 'empty-workspace';
          emptyDiv.innerHTML = `
            <p>Arraste os blocos para cá!</p>
            <img src="data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='100' height='100' viewBox='0 0 100 100'><circle cx='50' cy='50' r='40' fill='%23FFD166' stroke='%23FF9F1C' stroke-width='2'/><text x='50' y='55' font-family='Comic Sans MS' font-size='14' text-anchor='middle' fill='%236B2D42'>Solte aqui!</text></svg>">
          `;
          workspace.appendChild(emptyDiv);
        }
      }, 300);
    }
  </script>
</body>
</html>
)=====";

  page.replace("%BLOCKS%", generateBlocksHTML());
  page.replace("%IP%", WiFi.softAPIP().toString());
  webServer.send(200, "text/html", page);
}

void handleExecute() {
  if (isExecuting) {
    webServer.send(200, "text/plain", "Já está executando");
    return;
  }

  // Processar todos os argumentos recebidos
  blockCount = 0;
  for (int i = 0; i < 10; i++) {
    String type = webServer.arg("block_" + String(i) + "_type");
    String value = webServer.arg("block_" + String(i) + "_value");
    
    if (type == "") break;
    
    blocks[blockCount].type = type;
    blocks[blockCount].value = value;
    blockCount++;
    
    Serial.print("Bloco "); 
    Serial.print(i);
    Serial.print(": ");
    Serial.print(type);
    Serial.print(" -> ");
    Serial.println(value);
  }

  // Iniciar execução
  currentBlockIndex = 0;
  isExecuting = true;
  shouldStop = false;
  blockStartTime = millis(); // Importante: marcar o tempo inicial
  
  // Executar o primeiro bloco imediatamente
  executeCurrentBlock();
  
  webServer.send(200, "text/plain", "Execução iniciada");
}

void handleStop() {
  shouldStop = true;
  isExecuting = false;
  currentBlockIndex = -1;
  digitalWrite(PinLed, LOW);
  digitalWrite(Motor, LOW);
  webServer.send(200, "text/plain", "Parando execução");
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

// Função para executar o bloco atual
void executeCurrentBlock() {
  if (currentBlockIndex < 0 || currentBlockIndex >= blockCount) return;

  Block current = blocks[currentBlockIndex];
  
  if (current.type == "led") {
    bool state = (current.value.indexOf("acender") != -1);
    digitalWrite(PinLed, state ? HIGH : LOW);
    Serial.print("Executando LED ");
    Serial.println(state ? "LIGADO" : "DESLIGADO");
  } 
  else if (current.type == "motor") {
    bool state = (current.value.indexOf("ligar") != -1);
    digitalWrite(Motor, state ? HIGH : LOW);
    Serial.print("Executando Motor ");
    print_text(state ? "LIGADO" : "DESLIGADO",);
  }
  else if (current.type == "repeat") {
    delay(1000);
    Serial.println("Iniciando pausa...");
  }
  
  // Marcar o tempo de início independente do tipo de bloco
  blockStartTime = millis();
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
  shouldStop = true;
  blockCount = 0;
  digitalWrite(PinLed, LOW);
  digitalWrite(Motor, LOW);
  delay(100);
  shouldStop = false;
  handleRoot();
}

//para a execução que está em loop
void stopExecution() {
  isExecuting = false;
  currentBlockIndex = -1;
  digitalWrite(PinLed, LOW);
  digitalWrite(Motor, LOW);
  shouldStop = false;
  Serial.println("Execução parada");
}

//Atualiza a tela de informações tanto oled como teminal
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  
  display.println("ESP32 Amigo");
  display.println("Pronto para usar!");
  display.println("IP: " + WiFi.softAPIP().toString());
  
  display.display();
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
  
  // Desligar os dispositivos inicialmente
  digitalWrite(PinLed, LOW);
  digitalWrite(Motor, LOW);
  
  setupWiFiAP();
  dnsServer.start(53, "*", WiFi.softAPIP());
  
  webServer.on("/", handleRoot);
  webServer.on("/execute", HTTP_POST, handleExecute);
  webServer.on("/stop", HTTP_POST, handleStop);
  webServer.on("/remove", HTTP_POST, handleRemove);
  webServer.on("/reset", HTTP_POST, handleReset);
  webServer.begin();
  
  Serial.println("Servidor HTTP iniciado");
  updateDisplay();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();

  if (isExecuting && !shouldStop && currentBlockIndex >= 0) {
    Block current = blocks[currentBlockIndex];
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - blockStartTime;
    bool blockFinished = false;

    // Verificar se o bloco atual terminou
    if (current.type == "repeat") {
      // Converter valor do bloco para milissegundos
      int delayTime = 1000; // padrão 1 segundo
      if (current.value.indexOf("1s") != -1) delayTime = 1000;
      
      if (elapsedTime >= delayTime) {
        blockFinished = true;
      }
    } else {
      // Para outros blocos, esperar pelo menos 500ms
      if (elapsedTime >= 500) {
        blockFinished = true;
      }
    }

    // Avançar para o próximo bloco se necessário
    if (blockFinished) {
      currentBlockIndex++;
      
      if (currentBlockIndex >= blockCount) {
        if (shouldRepeat) {
          currentBlockIndex = 0; // Reiniciar do primeiro bloco
        } else {
          stopExecution();
          return;
        }
      }
      
      // Executar o próximo bloco somente quando for avançar
      executeCurrentBlock();
    }
  }

  delay(10); // Pequeno delay para evitar sobrecarga
}