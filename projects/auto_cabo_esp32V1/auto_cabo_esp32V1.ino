#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>

// Configurações do Wi-Fi
const char* ssid = "ESP32_Energy_Monitor";
const char* password = "12345678";

// Pinos
const int relayPin = 2;
const int acs712Pin = 34;  // GPIO34 (ADC1_CH6)

// Parâmetros do ACS712
const float sensitivity = 0.185; // 185mV/A para ACS712-5A
const float vRef = 3.3;         // Tensão de referência do ESP32
const int adcMax = 4095;        // ESP32 tem ADC de 12 bits

// Tarifa de energia (R$/Wh)
const float tariff = 0.00054933;

// Variáveis globais
WebServer server(80);
bool relayState = false;
float current = 0;
float power = 0;
float energy = 0;
float energyCost = 0;
unsigned long lastTime = 0;
unsigned long deviceOnTime = 0;
bool deviceConnected = false;
float zeroCurrentVoltage = 2.5; // Valor inicial (será calibrado)

void calibrateACS712() {
  Serial.println("Calibrando ACS712... (Nenhuma carga deve estar conectada)");
  float sum = 0;
  for (int i = 0; i < 200; i++) {
    sum += analogRead(acs712Pin);
    delay(10);
  }
  zeroCurrentVoltage = (sum / 200) * (vRef / adcMax);
  Serial.print("Tensão de zero corrente: ");
  Serial.print(zeroCurrentVoltage, 4);
  Serial.println("V");
}

void measureCurrent() {
  int rawValue = analogRead(acs712Pin);
  float voltage = (rawValue * (vRef / adcMax)) - zeroCurrentVoltage;
  
  current = voltage / sensitivity;
  
  // Filtro para pequenas correntes
  if (fabs(current) < 0.1) {
    current = 0;
    deviceConnected = false;
  } else {
    deviceConnected = true;
    current = fabs(current); // Usamos valor absoluto
  }
  
  power = current * 220.0; // Calcula potência assumindo 220V
  
  // Debug no Serial Monitor
  Serial.print("Raw: ");
  Serial.print(rawValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 4);
  Serial.print("V | Current: ");
  Serial.print(current, 3);
  Serial.println("A");
}

void updateEnergy() {
  unsigned long now = millis();
  if (lastTime == 0) { // Primeira execução
    lastTime = now;
    return;
  }
  
  unsigned long elapsedTime = now - lastTime;
  float hours = elapsedTime / 3600000.0; // ms para horas
  
  if (deviceConnected && relayState) {
    float deltaEnergy = power * hours;
    energy += deltaEnergy;
    energyCost += deltaEnergy * tariff;
    deviceOnTime += elapsedTime;
  }
  
  lastTime = now;
}

void handleRoot() {
  String html = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Monitor de Energia</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f5f5f5; }
      .container { max-width: 600px; margin: 0 auto; }
      .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); margin-bottom: 20px; }
      .btn { background: #4CAF50; color: white; border: none; padding: 10px 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; border-radius: 5px; width: 100%; }
      .btn-off { background: #f44336; }
      .status { font-weight: bold; }
      .data { margin-top: 15px; }
      .data-item { display: flex; justify-content: space-between; margin-bottom: 8px; }
      .data-label { font-weight: bold; }
      .device-connected { color: #4CAF50; }
      .device-disconnected { color: #f44336; }
      .cost { color: #2196F3; }
      h1 { color: #333; text-align: center; }
      h2 { color: #444; margin-top: 0; }
    </style>
    <script>
      function updateData() {
        fetch('/data')
          .then(response => response.json())
          .then(data => {
            // Atualiza estado do relé
            document.getElementById('relayStatus').textContent = data.relayState ? 'LIGADO' : 'DESLIGADO';
            document.getElementById('relayBtn').textContent = data.relayState ? 'DESLIGAR RELÉ' : 'LIGAR RELÉ';
            document.getElementById('relayBtn').className = data.relayState ? 'btn btn-off' : 'btn';
            
            // Atualiza medições
            updateMeasurement('current', data.current, ' A');
            updateMeasurement('power', data.power, ' W');
            updateMeasurement('energy', data.energy, ' Wh');
            updateMeasurement('energyCost', data.energyCost, ' R$', true);
            document.getElementById('onTime').textContent = formatTime(data.deviceOnTime);
            
            // Atualiza status do dispositivo
            const deviceStatus = document.getElementById('deviceStatus');
            if (data.deviceConnected) {
              deviceStatus.textContent = 'CONECTADO';
              deviceStatus.className = 'status device-connected';
            } else {
              deviceStatus.textContent = 'DESCONECTADO';
              deviceStatus.className = 'status device-disconnected';
            }
          });
      }
      
      function updateMeasurement(id, value, unit, isCurrency = false) {
        const element = document.getElementById(id);
        if (isCurrency) {
          element.textContent = 'R$ ' + value.toFixed(4);
        } else {
          element.textContent = value.toFixed(3) + unit;
        }
      }
      
      function formatTime(ms) {
        const hours = Math.floor(ms / 3600000);
        const minutes = Math.floor((ms % 3600000) / 60000);
        const seconds = Math.floor((ms % 60000) / 1000);
        return `${hours}h ${minutes}m ${seconds}s`;
      }
      
      function toggleRelay() {
        fetch('/relay')
          .then(response => response.text())
          .then(updateData);
      }
      
      function resetEnergy() {
        if(confirm('Tem certeza que deseja zerar o consumo e custo?')) {
          fetch('/reset')
            .then(response => response.text())
            .then(updateData);
        }
      }
      
      // Atualiza dados a cada segundo
      setInterval(updateData, 1000);
      
      // Carrega os dados quando a página é aberta
      window.onload = updateData;
    </script>
  </head>
  <body>
    <div class="container">
      <h1>Monitor de Energia</h1>
      
      <div class="card">
        <h2>Controle do Relé</h2>
        <div class="data-item">
          <span>Status:</span>
          <span id="relayStatus" class="status">--</span>
        </div>
        <button id="relayBtn" class="btn" onclick="toggleRelay()">--</button>
      </div>
      
      <div class="card">
        <h2>Monitoramento</h2>
        <div class="data-item">
          <span>Dispositivo:</span>
          <span id="deviceStatus" class="status">--</span>
        </div>
        
        <div class="data">
          <div class="data-item">
            <span>Corrente:</span>
            <span id="current">--</span>
          </div>
          <div class="data-item">
            <span>Potência:</span>
            <span id="power">--</span>
          </div>
          <div class="data-item">
            <span>Energia:</span>
            <span id="energy">--</span>
          </div>
          <div class="data-item">
            <span>Custo:</span>
            <span id="energyCost" class="cost">--</span>
          </div>
          <div class="data-item">
            <span>Tempo de uso:</span>
            <span id="onTime">--</span>
          </div>
        </div>
        
        <button class="btn" onclick="resetEnergy()" style="background: #FF9800;">ZERAR CONSUMO</button>
      </div>
    </div>
  </body>
  </html>
  )=====";
  
  server.send(200, "text/html", html);
}

void handleRelay() {
  relayState = !relayState;
  digitalWrite(relayPin, relayState ? HIGH : LOW);
  
  // Salva estado do relé na EEPROM
  EEPROM.write(0, relayState ? 1 : 0);
  EEPROM.commit();
  
  server.send(200, "text/plain", "Relé " + String(relayState ? "ligado" : "desligado"));
}

void handleData() {
  String json = "{";
  json += "\"relayState\":" + String(relayState ? "true" : "false") + ",";
  json += "\"current\":" + String(current) + ",";
  json += "\"power\":" + String(power) + ",";
  json += "\"energy\":" + String(energy) + ",";
  json += "\"energyCost\":" + String(energyCost) + ",";
  json += "\"deviceOnTime\":" + String(deviceOnTime) + ",";
  json += "\"deviceConnected\":" + String(deviceConnected ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleReset() {
  energy = 0;
  energyCost = 0;
  deviceOnTime = 0;
  server.send(200, "text/plain", "Consumo e custo zerados");
}

void handleCalibrate() {
  calibrateACS712();
  server.send(200, "text/plain", "Sensor calibrado. Tensão zero: " + String(zeroCurrentVoltage, 4) + "V");
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  
  // Configura pinos
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  
  // Carrega estado do relé da EEPROM
  relayState = EEPROM.read(0) == 1;
  digitalWrite(relayPin, relayState ? HIGH : LOW);
  
  // Configura Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // Configura rotas do servidor web
  server.on("/", handleRoot);
  server.on("/relay", handleRelay);
  server.on("/data", handleData);
  server.on("/reset", handleReset);
  server.on("/calibrate", handleCalibrate);
  
  server.begin();
  
  // Calibração inicial
  calibrateACS712();
  lastTime = millis();
}

void loop() {
  server.handleClient();
  
  static unsigned long lastMeasurement = 0;
  if (millis() - lastMeasurement >= 500) {
    measureCurrent();
    lastMeasurement = millis();
  }
  
  updateEnergy();
}