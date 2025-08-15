#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>

// Definição dos pinos
#define LED_AZUL 2      // LED azul da placa
#define SONAR_TRIG 12   // Pino trigger do sonar
#define SONAR_ECHO 13   // Pino echo do sonar
#define SERVO1 14       // Servo Motor 1
#define SERVO2 15       // Servo Motor 2
#define SERVO3 16       // Servo Motor 3
#define SERVO4 17       // Servo Motor 4
#define BUZZER 18       // Pino do buzzer

// Credenciais do WiFi
const char* ssid = "ESP32_Amigo";
const char* password = "brincando123";

WebServer server(80);
Servo servo1, servo2, servo3, servo4;

// HTML da interface
const char* htmlPage = R"rawliteral(
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
    .block.blue {
      background-color: #118AB2;
      color: white;
      border-color: #073B4C;
    }
    .block.purple {
      background-color: #A663CC;
      color: white;
      border-color: #6A2C70;
    }
    .block.yellow {
      background-color: #FFD166;
      color: #6B2D42;
      border-color: #EF476F;
    }
    .block.red {
      background-color: #EF476F;
      color: white;
      border-color: #B23A48;
    }
    .block.orange {
      background-color: #FF9F1C;
      color: white;
      border-color: #FF6B6B;
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
    select, input {
      padding: 6px;
      border-radius: 8px;
      border: 2px solid #4ECDC4;
      font-family: 'Comic Sans MS', cursive;
      margin: 5px 0;
    }
    input[type="number"] {
      width: 80px;
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
    .if-block {
      background-color: #FF9F1C;
      border-left: 6px solid #EF476F;
      padding: 10px;
      margin: 10px 0;
      border-radius: 0 10px 10px 0;
    }
    .if-condition {
      font-weight: bold;
      color: #EF476F;
    }
    .tone-control {
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 10px;
      margin: 5px 0;
    }
  </style>
</head>
<body>
  <div class="header">
    <h1>🤖 Brincando com Servos, Sonar e Buzzer 🎵</h1>
  </div>
  
  <div class="welcome-message">
    Olá, programador criativo! 👋<br>
    Monte seu programa com os blocos abaixo!
  </div>
  
  <div class="container">
    <div class="block-palette">
      <h2>📦 Caixa de Blocos</h2>
      
      <div class="block green" draggable="true" data-type="led" id="led-block">
        <span class="object-icon">💡</span> Luz:
        <select id="led-action">
          <option value="on">Ligar 🔦</option>
          <option value="off">Desligar 🌙</option>
        </select>
      </div>
      
      <div class="block blue" draggable="true" data-type="servo" id="servo-block">
        <span class="object-icon">⚙️</span> Servo:
        <select id="servo-number">
          <option value="1">Servo 1</option>
          <option value="2">Servo 2</option>
          <option value="3">Servo 3</option>
          <option value="4">Servo 4</option>
        </select>
        <br>
        Ângulo: <input type="number" id="servo-angle" min="0" max="180" value="90">
      </div>
      
      <div class="block purple" draggable="true" data-type="sonar" id="sonar-block">
        <span class="object-icon">📡</span> Sonar:
        <select id="sonar-action">
          <option value="on">Ligar</option>
          <option value="off">Desligar</option>
        </select>
      </div>
      
      <div class="block orange" draggable="true" data-type="buzzer" id="buzzer-block">
        <span class="object-icon">🎵</span> Buzzer:
        <select id="buzzer-action">
          <option value="on">Ligar</option>
          <option value="off">Desligar</option>
        </select>
        <div class="tone-control">
          Frequência: <input type="number" id="buzzer-freq" min="31" max="8000" value="1000"> Hz
        </div>
        <div class="tone-control">
          Duração: <input type="number" id="buzzer-duration" min="100" max="5000" value="500"> ms
        </div>
      </div>
      
      <div class="block yellow" draggable="true" data-type="if" id="if-block">
        <span class="object-icon">❓</span> SE distância
        <select id="if-operator">
          <option value="<">menor que</option>
          <option value=">">maior que</option>
          <option value="=="">igual a</option>
        </select>
        <input type="number" id="if-distance" min="2" max="400" value="10"> cm
      </div>
      
      <div class="block red" draggable="true" data-type="delay" id="delay-block">
        <span class="object-icon">⏱️</span> Pausa:
        <input type="number" id="delay-time" min="100" max="10000" value="1000"> ms
      </div>
    </div>
    
    <div class="workspace" id="workspace">
      <h2>🌟 Área de Programação</h2>
      <div class="empty-workspace">
        <p>Arraste os blocos para cá!</p>
        <img src="data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='100' height='100' viewBox='0 0 100 100'><circle cx='50' cy='50' r='40' fill='%23FFD166' stroke='%23FF9F1C' stroke-width='2'/><text x='50' y='55' font-family='Comic Sans MS' font-size='14' text-anchor='middle' fill='%236B2D42'>Solte aqui!</text></svg>">
      </div>
    </div>
  </div>
  
  <button id="run-btn">🚀 Executar Programa</button>
  
  <div class="device-status">
    <h3>📱 Status do ESP32</h3>
    <p id="status">😊 Pronto para programar!</p>
    <p>🌐 Endereço: <span id="ip">%IP_ADDRESS%</span></p>
    <p>📡 Distância: <span id="distance">--</span> cm</p>
  </div>

  <script>
    document.addEventListener('DOMContentLoaded', function() {
      const workspace = document.getElementById('workspace');
      const emptyWorkspace = workspace.querySelector('.empty-workspace');
      const runBtn = document.getElementById('run-btn');
      const statusElement = document.getElementById('status');
      const distanceElement = document.getElementById('distance');
      
      // Atualizar IP
      document.getElementById('ip').textContent = window.location.hostname;
      
      // Atualizar distância do sonar periodicamente
      function updateDistance() {
        fetch('/distance')
          .then(response => response.json())
          .then(data => {
            distanceElement.textContent = data.distance || '--';
            setTimeout(updateDistance, 1000);
          })
          .catch(() => {
            distanceElement.textContent = '--';
            setTimeout(updateDistance, 2000);
          });
      }
      updateDistance();
      
      // Permitir que blocos sejam arrastados
      document.querySelectorAll('.block').forEach(block => {
        block.addEventListener('dragstart', function(e) {
          e.dataTransfer.setData('text/plain', this.dataset.type);
          e.dataTransfer.effectAllowed = 'copy';
        });
      });
      
      // Permitir soltar blocos na área de trabalho
      workspace.addEventListener('dragover', function(e) {
        e.preventDefault();
        e.dataTransfer.dropEffect = 'copy';
      });
      
      workspace.addEventListener('drop', function(e) {
        e.preventDefault();
        const blockType = e.dataTransfer.getData('text/plain');
        
        // Criar novo bloco baseado no tipo
        let newBlock;
        switch(blockType) {
          case 'led':
            newBlock = createLedBlock();
            break;
          case 'servo':
            newBlock = createServoBlock();
            break;
          case 'sonar':
            newBlock = createSonarBlock();
            break;
          case 'buzzer':
            newBlock = createBuzzerBlock();
            break;
          case 'if':
            newBlock = createIfBlock();
            break;
          case 'delay':
            newBlock = createDelayBlock();
            break;
        }
        
        // Animação ao adicionar
        newBlock.style.transform = 'scale(0)';
        workspace.appendChild(newBlock);
        setTimeout(() => {
          newBlock.style.transform = 'scale(1)';
        }, 10);
        
        // Esconder mensagem inicial
        if(emptyWorkspace.style.display !== 'none') {
          emptyWorkspace.style.display = 'none';
        }
      });
      
      // Funções para criar blocos na área de trabalho
      function createLedBlock() {
        const original = document.getElementById('led-block');
        const newBlock = original.cloneNode(true);
        newBlock.id = '';
        newBlock.draggable = false;
        newBlock.style.cursor = 'default';
        newBlock.style.boxShadow = '2px 2px 0px rgba(0,0,0,0.2)';
        addRemoveButton(newBlock);
        return newBlock;
      }
      
      function createServoBlock() {
        const original = document.getElementById('servo-block');
        const newBlock = original.cloneNode(true);
        newBlock.id = '';
        newBlock.draggable = false;
        newBlock.style.cursor = 'default';
        newBlock.style.boxShadow = '2px 2px 0px rgba(0,0,0,0.2)';
        addRemoveButton(newBlock);
        return newBlock;
      }
      
      function createSonarBlock() {
        const original = document.getElementById('sonar-block');
        const newBlock = original.cloneNode(true);
        newBlock.id = '';
        newBlock.draggable = false;
        newBlock.style.cursor = 'default';
        newBlock.style.boxShadow = '2px 2px 0px rgba(0,0,0,0.2)';
        addRemoveButton(newBlock);
        return newBlock;
      }
      
      function createBuzzerBlock() {
        const original = document.getElementById('buzzer-block');
        const newBlock = original.cloneNode(true);
        newBlock.id = '';
        newBlock.draggable = false;
        newBlock.style.cursor = 'default';
        newBlock.style.boxShadow = '2px 2px 0px rgba(0,0,0,0.2)';
        addRemoveButton(newBlock);
        return newBlock;
      }
      
      function createIfBlock() {
        const original = document.getElementById('if-block');
        const newBlock = original.cloneNode(true);
        newBlock.id = '';
        newBlock.draggable = false;
        newBlock.style.cursor = 'default';
        newBlock.style.boxShadow = '2px 2px 0px rgba(0,0,0,0.2)';
        
        // Adicionar área para blocos condicionais
        const ifContent = document.createElement('div');
        ifContent.className = 'if-content';
        ifContent.style.margin = '10px 0 10px 20px';
        ifContent.style.paddingLeft = '10px';
        ifContent.style.borderLeft = '3px dashed #EF476F';
        
        const ifLabel = document.createElement('div');
        ifLabel.className = 'if-label';
        ifLabel.textContent = 'ENTÃO:';
        ifLabel.style.fontWeight = 'bold';
        ifLabel.style.color = '#EF476F';
        ifLabel.style.marginBottom = '5px';
        
        const dropZone = document.createElement('div');
        dropZone.className = 'if-drop-zone';
        dropZone.style.minHeight = '50px';
        dropZone.style.background = 'rgba(239, 71, 111, 0.1)';
        dropZone.style.borderRadius = '5px';
        dropZone.style.padding = '5px';
        dropZone.style.textAlign = 'center';
        dropZone.textContent = 'Arraste blocos aqui';
        
        // Configurar zona de soltar
        dropZone.addEventListener('dragover', function(e) {
          e.preventDefault();
          e.dataTransfer.dropEffect = 'copy';
          this.style.background = 'rgba(239, 71, 111, 0.3)';
        });
        
        dropZone.addEventListener('dragleave', function() {
          this.style.background = 'rgba(239, 71, 111, 0.1)';
        });
        
        dropZone.addEventListener('drop', function(e) {
          e.preventDefault();
          this.style.background = 'rgba(239, 71, 111, 0.1)';
          
          const blockType = e.dataTransfer.getData('text/plain');
          let newBlock;
          
          switch(blockType) {
            case 'led':
              newBlock = createLedBlock();
              break;
            case 'servo':
              newBlock = createServoBlock();
              break;
            case 'sonar':
              newBlock = createSonarBlock();
              break;
            case 'buzzer':
              newBlock = createBuzzerBlock();
              break;
            case 'delay':
              newBlock = createDelayBlock();
              break;
          }
          
          this.insertBefore(newBlock, this.lastChild);
          if(this.textContent.includes('Arraste blocos aqui')) {
            this.textContent = '';
          }
        });
        
        ifContent.appendChild(ifLabel);
        ifContent.appendChild(dropZone);
        newBlock.appendChild(ifContent);
        addRemoveButton(newBlock);
        
        return newBlock;
      }
      
      function createDelayBlock() {
        const original = document.getElementById('delay-block');
        const newBlock = original.cloneNode(true);
        newBlock.id = '';
        newBlock.draggable = false;
        newBlock.style.cursor = 'default';
        newBlock.style.boxShadow = '2px 2px 0px rgba(0,0,0,0.2)';
        addRemoveButton(newBlock);
        return newBlock;
      }
      
      function addRemoveButton(block) {
        const removeBtn = document.createElement('button');
        removeBtn.textContent = '❌';
        removeBtn.style.float = 'right';
        removeBtn.style.background = 'none';
        removeBtn.style.border = 'none';
        removeBtn.style.fontSize = '18px';
        removeBtn.style.cursor = 'pointer';
        removeBtn.onclick = function() { 
          block.style.transform = 'scale(0)';
          setTimeout(() => {
            block.remove();
            if(workspace.querySelectorAll('.block').length === 0) {
              emptyWorkspace.style.display = 'block';
            }
          }, 300);
        };
        block.appendChild(removeBtn);
      }
      
      // Enviar programa para o ESP32
      runBtn.addEventListener('click', function() {
        const blocks = workspace.querySelectorAll('.block:not(.if-content .block)');
        if(blocks.length === 0) {
          alert('Por favor, adicione alguns blocos primeiro!');
          return;
        }
        
        const program = [];
        
        blocks.forEach(block => {
          const type = block.dataset.type;
          let command;
          
          switch(type) {
            case 'led':
              const ledAction = block.querySelector('select').value;
              command = {
                type: 'led',
                action: ledAction
              };
              break;
              
            case 'servo':
              const servoNumber = block.querySelector('#servo-number').value;
              const servoAngle = block.querySelector('#servo-angle').value;
              command = {
                type: 'servo',
                servo: servoNumber,
                angle: servoAngle
              };
              break;
              
            case 'sonar':
              const sonarAction = block.querySelector('select').value;
              command = {
                type: 'sonar',
                action: sonarAction
              };
              break;
              
            case 'buzzer':
              const buzzerAction = block.querySelector('#buzzer-action').value;
              const buzzerFreq = block.querySelector('#buzzer-freq').value;
              const buzzerDuration = block.querySelector('#buzzer-duration').value;
              command = {
                type: 'buzzer',
                action: buzzerAction,
                frequency: buzzerFreq,
                duration: buzzerDuration
              };
              break;
              
            case 'if':
              const ifOperator = block.querySelector('#if-operator').value;
              const ifDistance = block.querySelector('#if-distance').value;
              
              // Coletar comandos dentro do bloco if
              const ifCommands = [];
              const ifContent = block.querySelector('.if-content .if-drop-zone');
              ifContent.querySelectorAll('.block').forEach(ifBlock => {
                const ifType = ifBlock.dataset.type;
                let ifCommand;
                
                switch(ifType) {
                  case 'led':
                    const ifLedAction = ifBlock.querySelector('select').value;
                    ifCommand = {
                      type: 'led',
                      action: ifLedAction
                    };
                    break;
                    
                  case 'servo':
                    const ifServoNumber = ifBlock.querySelector('#servo-number').value;
                    const ifServoAngle = ifBlock.querySelector('#servo-angle').value;
                    ifCommand = {
                      type: 'servo',
                      servo: ifServoNumber,
                      angle: ifServoAngle
                    };
                    break;
                    
                  case 'buzzer':
                    const ifBuzzerAction = ifBlock.querySelector('#buzzer-action').value;
                    const ifBuzzerFreq = ifBlock.querySelector('#buzzer-freq').value;
                    const ifBuzzerDuration = ifBlock.querySelector('#buzzer-duration').value;
                    ifCommand = {
                      type: 'buzzer',
                      action: ifBuzzerAction,
                      frequency: ifBuzzerFreq,
                      duration: ifBuzzerDuration
                    };
                    break;
                    
                  case 'delay':
                    const ifDelayTime = ifBlock.querySelector('input').value;
                    ifCommand = {
                      type: 'delay',
                      time: ifDelayTime
                    };
                    break;
                }
                
                if(ifCommand) ifCommands.push(ifCommand);
              });
              
              command = {
                type: 'if',
                operator: ifOperator,
                distance: ifDistance,
                commands: ifCommands
              };
              break;
              
            case 'delay':
              const delayTime = block.querySelector('input').value;
              command = {
                type: 'delay',
                time: delayTime
              };
              break;
          }
          
          if(command) program.push(command);
        });
        
        // Enviar para o ESP32
        runBtn.disabled = true;
        runBtn.textContent = '⏳ Executando...';
        statusElement.innerHTML = '🤖 Executando seu programa...';
        
        fetch('/run', {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({program: program})
        })
        .then(response => response.json())
        .then(data => {
          if(data.success) {
            runBtn.textContent = '🎉 Programa concluído!';
            statusElement.innerHTML = '✅ Programa executado com sucesso!';
            setTimeout(() => {
              runBtn.textContent = '🚀 Executar Novamente';
              statusElement.innerHTML = '😊 Pronto para programar!';
              runBtn.disabled = false;
            }, 2000);
          } else {
            throw new Error(data.message || 'Erro desconhecido');
          }
        })
        .catch(error => {
          console.error('Error:', error);
          runBtn.textContent = '😢 Oops! Algo deu errado';
          statusElement.innerHTML = '❌ Erro: ' + error.message;
          setTimeout(() => {
            runBtn.textContent = '🚀 Tentar novamente';
            statusElement.innerHTML = '😊 Pronto para programar!';
            runBtn.disabled = false;
          }, 2000);
        });
      });
    });
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  String page = htmlPage;
  page.replace("%IP_ADDRESS%", WiFi.softAPIP().toString());
  server.send(200, "text/html", page);
}

// Função para medir distância com o sonar
float getDistance() {
  digitalWrite(SONAR_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(SONAR_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG, LOW);
  
  long duration = pulseIn(SONAR_ECHO, HIGH);
  float distance = duration * 0.034 / 2;
  
  return distance;
}

void handleGetDistance() {
  float distance = getDistance();
  String response = "{\"distance\":" + String(distance) + "}";
  server.send(200, "application/json", response);
}

void handleRunProgram() {
  if (server.hasArg("plain")) {
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    
    if (error) {
      server.send(400, "application/json", "{\"success\":false,\"message\":\"Erro ao processar JSON\"}");
      return;
    }
    
    JsonArray program = doc["program"];
    bool sonarEnabled = false;
    
    // Executa cada comando do programa
    for (JsonObject cmd : program) {
      const char* type = cmd["type"];
      
      if (strcmp(type, "led") == 0) {
        const char* action = cmd["action"];
        digitalWrite(LED_AZUL, strcmp(action, "on") == 0 ? HIGH : LOW);
      }
      else if (strcmp(type, "servo") == 0) {
        const char* servo = cmd["servo"];
        int angle = cmd["angle"];
        
        switch (atoi(servo)) {
          case 1: servo1.write(angle); break;
          case 2: servo2.write(angle); break;
          case 3: servo3.write(angle); break;
          case 4: servo4.write(angle); break;
        }
      }
      else if (strcmp(type, "sonar") == 0) {
        const char* action = cmd["action"];
        sonarEnabled = strcmp(action, "on") == 0;
      }
      else if (strcmp(type, "buzzer") == 0) {
        const char* action = cmd["action"];
        if (strcmp(action, "on") == 0) {
          int frequency = cmd["frequency"];
          int duration = cmd["duration"];
          tone(BUZZER, frequency, duration);
          delay(duration); // Espera a duração do tom
        } else {
          noTone(BUZZER);
        }
      }
      else if (strcmp(type, "if") == 0) {
        if (sonarEnabled) {
          const char* op = cmd["operator"];
          int conditionDistance = cmd["distance"];
          float currentDistance = getDistance();
          bool conditionMet = false;
          
          // Verifica a condição
          if (strcmp(op, "<") == 0) {
            conditionMet = currentDistance < conditionDistance;
          } else if (strcmp(op, ">") == 0) {
            conditionMet = currentDistance > conditionDistance;
          } else if (strcmp(op, "==") == 0) {
            conditionMet = abs(currentDistance - conditionDistance) < 2; // Margem de erro
          }
          
          // Executa comandos dentro do if se a condição for verdadeira
          if (conditionMet) {
            JsonArray ifCommands = cmd["commands"];
            for (JsonObject ifCmd : ifCommands) {
              const char* ifType = ifCmd["type"];
              
              if (strcmp(ifType, "led") == 0) {
                const char* action = ifCmd["action"];
                digitalWrite(LED_AZUL, strcmp(action, "on") == 0 ? HIGH : LOW);
              }
              else if (strcmp(ifType, "servo") == 0) {
                const char* servo = ifCmd["servo"];
                int angle = ifCmd["angle"];
                
                switch (atoi(servo)) {
                  case 1: servo1.write(angle); break;
                  case 2: servo2.write(angle); break;
                  case 3: servo3.write(angle); break;
                  case 4: servo4.write(angle); break;
                }
              }
              else if (strcmp(ifType, "buzzer") == 0) {
                const char* action = ifCmd["action"];
                if (strcmp(action, "on") == 0) {
                  int frequency = ifCmd["frequency"];
                  int duration = ifCmd["duration"];
                  tone(BUZZER, frequency, duration);
                  delay(duration);
                } else {
                  noTone(BUZZER);
                }
              }
              else if (strcmp(ifType, "delay") == 0) {
                int time = ifCmd["time"];
                delay(time);
              }
            }
          }
        }
      }
      else if (strcmp(type, "delay") == 0) {
        int time = cmd["time"];
        delay(time);
      }
    }
    
    server.send(200, "application/json", "{\"success\":true}");
  } else {
    server.send(400, "application/json", "{\"success\":false,\"message\":\"Nenhum dado recebido\"}");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Configura os pinos
  pinMode(LED_AZUL, OUTPUT);
  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Configura os servos
  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo3.attach(SERVO3);
  servo4.attach(SERVO4);
  
  // Posição inicial dos servos
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
  servo4.write(90);
  
  // Inicializa LED e buzzer desligados
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(BUZZER, LOW);
  
  // Cria o ponto de acesso
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  // Configura as rotas
  server.on("/", handleRoot);
  server.on("/run", HTTP_POST, handleRunProgram);
  server.on("/distance", handleGetDistance);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

